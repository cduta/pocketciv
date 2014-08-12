#include "Instruction.hpp"

#include <iostream>

Instruction::Instruction(QObject *parent)
    : QObject(parent), keep(false)
{}

Instruction::~Instruction()
{}

void Instruction::initInstruction()
{
    return;
}

Instruction *Instruction::triggerHex(Qt::MouseButton, int, int)
{
    return this;
}

Instruction *Instruction::triggerDone()
{
    return this;
}

Instruction *Instruction::getFollowingInstruction()
{
    // Ignored, since there is no previous instruction in most instructions.
    return this;
}

Instruction *Instruction::triggerBuildCity()
{
    return this;
}

Instruction *Instruction::triggerBuildFarm()
{
    return this;
}

bool Instruction::keepInstruction()
{
    return this->keep;
}

void Instruction::setKeepInstruction(bool keep)
{
    this->keep = keep;
    return;
}
