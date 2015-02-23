#include "BuildCityInstruction.hpp"

BuildCityInstruction::BuildCityInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction)
{}

void BuildCityInstruction::initInstruction()
{
    this->boardModel->sendMessage("BUILD CITY:");
    this->boardModel->sendMessage("Choose a region without a city.");
    this->boardModel->sendMessage("Decimate 4 tribes on it to build a city with 1 AV.");
    this->boardModel->sendMessage("At least 1 tribe has to remain in the region.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();
    return;
}

Instruction *BuildCityInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(!regionModel->hasCity() && regionModel->getTribes() >= 4+1)
        {
            regionModel->setTribes(regionModel->getTribes() - 4);
            regionModel->setCityAV(1);
            regionModel->setCity(true);
        }
    }

    return this;
}

Instruction *BuildCityInstruction::triggerDone()
{
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();

    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}


