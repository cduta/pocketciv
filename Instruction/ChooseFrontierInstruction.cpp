#include "ChooseFrontierInstruction.hpp"

#include "Instruction/InitialMountainInstruction.hpp"

ChooseFrontierInstruction::ChooseFrontierInstruction(BoardModel *boardModel)
    : Instruction(boardModel)
{
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Choose the frontier hexes.");
    this->boardModel->sendMessage("The other hexes are considered sea hexes.");
    this->boardModel->sendMessage("Seas seperated by regions or frontier are considered seperate seas.");
    this->boardModel->sendMessage("When you are done, press Done...");
    this->boardModel->initialRegionModels();
    this->boardModel->enableAllHexes();
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
    return new InitialMountainInstruction(this->boardModel);
}
