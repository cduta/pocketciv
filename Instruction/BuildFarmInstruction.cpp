#include "BuildFarmInstruction.hpp"

BuildFarmInstruction::BuildFarmInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction)
{
    this->nextInstruction->setKeepInstruction(true);
}

void BuildFarmInstruction::initInstruction()
{
    this->boardModel->sendMessage("BUILD FARM:");
    this->boardModel->sendMessage("Choose a region without a farm and with a forest.");
    this->boardModel->sendMessage("Decimate 2 tribes and the forest to build a farm.");
    this->boardModel->sendMessage("At least 1 tribe has to remain in the region.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();
    return;
}

Instruction *BuildFarmInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(!regionModel->hasFarm() && regionModel->getTribes() >= 2+1 && regionModel->hasForest())
        {
            regionModel->setTribes(regionModel->getTribes() - 2);
            regionModel->setForest(false);
            regionModel->setFarm(true);
        }
    }

    return this;
}

Instruction *BuildFarmInstruction::triggerDone()
{
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();

    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
