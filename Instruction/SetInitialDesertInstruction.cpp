#include "SetInitialDesertInstruction.hpp"

#include "Instruction/PlaceInitialTribes.hpp"

SetInitialDesertInstruction::SetInitialDesertInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void SetInitialDesertInstruction::initInstruction()
{
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("No regions without mountain and/or forest found to place a desert.");
    this->boardModel->printMessage("Place ONE desert into one region without a forest.");
    this->boardModel->printMessage("When you are done, press Done...");
}

Instruction *SetInitialDesertInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(!regionModel->hasForest())
        {
            foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
            {
                regionModel->setDesert(false);
            }

            if(regionModel != NULL)
            {
                regionModel->setDesert(true);
            }
        }
    }

    return this;
}

Instruction *SetInitialDesertInstruction::triggerDone()
{
    if(this->boardModel->getDesertCount() == 1)
    {
        Instruction *next = new PlaceInitialTribes(this->boardModel);
        next->initInstruction();
        return next;
    }
    else
    {
        this->boardModel->printMessage("No desert set.");
    }

    return this;
}
