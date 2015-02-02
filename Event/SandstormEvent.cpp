#include "SandstormEvent.hpp"

#include "Instruction/Event/SandstormEventInstruction.hpp"

SandstormEvent::SandstormEvent(BoardModel *boardModel)
    : Event(), boardModel(boardModel)
{}

QString SandstormEvent::getEventName() const
{
    return "SANDSTORM";
}

Instruction *SandstormEvent::happen(Instruction *next) const
{
    return new SandstormEventInstruction(this->boardModel, next, this);
}
