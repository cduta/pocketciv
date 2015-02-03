#include "VolcanoEvent.hpp"

#include "Instruction/Event/VolcanoEventInstruction.hpp"

VolcanoEvent::VolcanoEvent(BoardModel *boardModel)
    : Event(), boardModel(boardModel)
{}

QString VolcanoEvent::getEventName() const
{
    return "VOLCANO";
}

Instruction *VolcanoEvent::happen(Instruction *next) const
{
    return new VolcanoEventInstruction(this->boardModel, next, this);
}
