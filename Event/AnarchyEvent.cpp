#include "AnarchyEvent.hpp"

#include "Instruction/Event/AnarchyEventInstruction.hpp"

AnarchyEvent::AnarchyEvent(BoardModel *boardModel)
    : Event(), boardModel(boardModel)
{}

QString AnarchyEvent::getEventName() const
{
    return "ANARCHY";
}

Instruction *AnarchyEvent::happen(Instruction *next) const
{
    return new AnarchyEventInstruction(this->boardModel, next, this);
}
