#include "Instruction.hpp"

#include <iostream>

Instruction::Instruction(BoardModel *boardModel)
    : boardModel(boardModel)
{}

Instruction::~Instruction()
{}

Instruction *Instruction::triggerHex(Qt::MouseButton, int, int)
{
    return this;
}

Instruction *Instruction::triggerDone()
{
    return this;
}
