#include "SetInitialDesertInstruction.hpp"

SetInitialDesertInstruction::SetInitialDesertInstruction(BoardModel *boardModel, QObject *parent)
    : Instruction(boardModel, parent)
{}

Instruction *SetInitialDesertInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(!regionModel->hasForest())
        {
            foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
            {
                regionModel->unsetDesert();
            }

            if(regionModel != NULL)
            {
                regionModel->setDesert();
            }
        }
    }

    return this;
}

Instruction *SetInitialDesertInstruction::triggerDone()
{
    if(this->boardModel->getDesertCount() == 1)
    {
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Place 3 Tribes into any amount of regions.");
        this->boardModel->sendMessage("When you are done, press Done....");
        return this;
    }
    else
    {
        this->boardModel->sendMessage("No desert set.");
    }

    return this;
}
