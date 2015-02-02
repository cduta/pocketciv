#include "TribalWarEvent.hpp"

#include "Instruction/Event/TribalWarEventInstruction.hpp"

TribalWarEvent::TribalWarEvent(BoardModel *boardModel)
    : Event(), boardModel(boardModel)
{}

QString TribalWarEvent::getEventName() const
{
    return "TRIBAL WAR";
}

Instruction *TribalWarEvent::happen(Instruction *next) const
{
    return new TribalWarEventInstruction(this->boardModel, next, this);
}
