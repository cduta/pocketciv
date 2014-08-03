#include "Instruction.hpp"

#include <iostream>

Instruction::Instruction(BoardModel *boardModel, QObject *parent)
    : QObject(parent), boardModel(boardModel)
{}

Instruction *Instruction::triggerHex(Qt::MouseButton, int, int)
{
    return this;
}

Instruction *Instruction::triggerDone()
{
    return this;
}
