#include "SuperstitionEvent.hpp"

#include "Instruction/Event/SuperstitionEventInstruction.hpp"

SuperstitionEvent::SuperstitionEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel)
    : Event(redCircles, greenSquares, blueHexagons), boardModel(boardModel)
{}

QString SuperstitionEvent::getEventName() const
{
    return "SUPERSTITION";
}

Instruction *SuperstitionEvent::happen(Instruction *next) const
{
    return new SuperstitionEventInstruction(this->boardModel, next, this);
}
