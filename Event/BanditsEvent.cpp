#include "BanditsEvent.hpp"

#include "Instruction/Event/BanditsEventInstruction.hpp"

BanditsEvent::BanditsEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel)
    : Event(redCircles, greenSquares, blueHexagons), boardModel(boardModel)
{}

QString BanditsEvent::getEventName() const
{
    return "BANDITS";
}

Instruction *BanditsEvent::happen(Instruction *next) const
{
    return new BanditsEventInstruction(this->boardModel, next, this);
}
