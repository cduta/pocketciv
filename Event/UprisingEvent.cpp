#include "UprisingEvent.hpp"

#include "Instruction/Event/UprisingEventInstruction.hpp"

UprisingEvent::UprisingEvent(BoardModel *boardModel)
    : Event(), boardModel(boardModel)
{}

QString UprisingEvent::getEventName() const
{
    return "UPRISING";
}

Instruction *UprisingEvent::happen(Instruction *next) const
{
    return new UprisingEventInstruction(this->boardModel, next, this);
}
