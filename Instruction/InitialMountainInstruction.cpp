#include "InitialMountainInstruction.hpp"

#include "Instruction/InitialForestInstruction.hpp"

InitialMountainInstruction::InitialMountainInstruction(BoardModel *boardModel, QObject *parent)
    : Instruction(boardModel, parent)
{}

Instruction *InitialMountainInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(regionModel != NULL)
        {
            regionModel->toggleMountain();
        }
    }

    return this;
}

Instruction *InitialMountainInstruction::triggerDone()
{
    if(this->boardModel->getMountainCount() == 5)
    {
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Place 5 forests on 5 different regions.");
        this->boardModel->sendMessage("When you are done, press Done...");
        this->deleteLater();
        return new InitialForestInstruction(this->boardModel);
    }
    else
    {
        if(this->boardModel->getMountainCount() < 5)
        {
            this->boardModel->sendMessage("Not enough mountains placed.");
        }
        else
        {
            this->boardModel->sendMessage("Too many mountains placed.");
        }
    }

    return this;
}
