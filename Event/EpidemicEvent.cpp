#include "EpidemicEvent.hpp"

#include "Instruction/Event/EpidemicEventInstruction.hpp"

EpidemicEvent::EpidemicEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel)
    : Event(redCircles, greenSquares, blueHexagons), boardModel(boardModel)
{}

QString EpidemicEvent::getEventName() const
{
    return "EPIDEMIC";
}

Instruction *EpidemicEvent::happen(Instruction *next) const
{
    return new EpidemicEventInstruction(this->boardModel, next, this);
}
