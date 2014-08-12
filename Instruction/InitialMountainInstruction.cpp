#include "InitialMountainInstruction.hpp"

#include "Instruction/InitialForestInstruction.hpp"

InitialMountainInstruction::InitialMountainInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void InitialMountainInstruction::initInstruction()
{
    this->boardModel->setUnsetHexesToSea();
    this->boardModel->groupSeas();
    this->boardModel->setChoosingHexesDone();
    this->boardModel->enableRegionSelectableHexes();
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Place 5 mountains on 5 different regions.");
    this->boardModel->sendMessage("When you are done, press Done...");
}

Instruction *InitialMountainInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(regionModel != NULL)
        {
            int mountainCount = this->boardModel->getMountainCount();
            if(mountainCount < 5)
            {
                regionModel->toggleMountain();
            }
            else if(mountainCount == 5)
            {
                regionModel->setMountain(false);
            }
        }
    }

    return this;
}

Instruction *InitialMountainInstruction::triggerDone()
{
    if(this->boardModel->getMountainCount() == 5)
    {
        Instruction *next = new InitialForestInstruction(this->boardModel);
        next->initInstruction();
        return next;
    }
    else
    {
        this->boardModel->sendMessage("Not enough mountains placed.");
    }

    return this;
}
