#include "CorruptionEvent.hpp"

#include "Instruction/Event/CorruptionEventInstruction.hpp"

CorruptionEvent::CorruptionEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel)
    : Event(redCircles, greenSquares, blueHexagons), boardModel(boardModel)
{}

QString CorruptionEvent::getEventName() const
{
    return "CORRUPTION";
}

Instruction *CorruptionEvent::happen(Instruction *next) const
{
    return new CorruptionEventInstruction(this->boardModel, next, this);
}
