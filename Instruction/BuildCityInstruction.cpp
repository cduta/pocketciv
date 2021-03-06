#include "BuildCityInstruction.hpp"

BuildCityInstruction::BuildCityInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), tribesCost(4)
{
    this->nextInstruction->setKeepInstruction(true);
}

void BuildCityInstruction::initInstruction()
{
    this->boardModel->printMessage("BUILD CITY:");
    this->boardModel->printMessage("Choose a region without a city.");
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::COMMON_TONGUE))
    {
        this->tribesCost = 2;
        this->boardModel->printMessage("Advance (COMMON TONGUE):");
    }
    this->boardModel->printMessage(QString("Decimate %1 tribes on it to build a city with 1 AV.")
                                  .arg(this->tribesCost));
    this->boardModel->printMessage("Remember: At least 1 tribe has to remain anywhere in the Empire, when decimating Tribes!");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("When you are done, press Done.");
    this->boardModel->printMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->setDoneButton(true);
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
        if(!regionModel->hasCity() && regionModel->getTribes() >= 4 && this->boardModel->getTribeCount() > 4)
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

    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}


