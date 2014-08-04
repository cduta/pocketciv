#include "ChooseFrontierInstruction.hpp"

#include "Instruction/InitialMountainInstruction.hpp"

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
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Place 5 mountains on 5 different regions.");
    this->boardModel->sendMessage("When you are done, press Done...");
    this->deleteLater();
    return new InitialMountainInstruction(this->boardModel);
}
