#include "FamineEvent.hpp"

#include "Instruction/Event/FamineEventInstruction.hpp"

FamineEvent::FamineEvent(BoardModel *boardModel)
    : Event(), boardModel(boardModel)
{}

QString FamineEvent::getEventName() const
{
    return "FAMINE";
}

Instruction *FamineEvent::happen(Instruction *next) const
{
    return new FamineEventInstruction(this->boardModel, next, this);
}

