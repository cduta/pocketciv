#include "ChooseFrontierInstruction.hpp"

ChooseFrontierInstruction::ChooseFrontierInstruction(BoardModel *boardModel, QObject *parent)
    : Instruction(boardModel, parent)
{
}

Instruction *ChooseFrontierInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        if(this->boardModel->refHexModel(x,y)->getRegion() == -1 || this->boardModel->refHexModel(x,y)->isFrontier())
        {
            this->boardModel->refHexModel(x,y)->toggleFrontier();
        }
    }

    return this;
}

Instruction *ChooseFrontierInstruction::triggerDone()
{
    this->boardModel->setUnsetHexesToSea();
    this->boardModel->groupSeas();
    this->boardModel->setChoosingHexesDone();
    this->boardModel->enableRegionSelectableHexes();
    return this;
}
