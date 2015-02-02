#include "FloodEvent.hpp"

#include "Instruction/Event/FloodEventInstruction.hpp"

FloodEvent::FloodEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel)
    : Event(redCircles, greenSquares, blueHexagons), boardModel(boardModel)
{}

QString FloodEvent::getEventName() const
{
    return "FLOOD";
}

Instruction *FloodEvent::happen(Instruction *next) const
{
    return new FloodEventInstruction(this->boardModel, next, this);
}
