#include "EpidemicEvent.hpp"

#include "Instruction/Event/EpidemicEventInstruction.hpp"

EpidemicEvent::EpidemicEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel)
    : Event(), boardModel(boardModel)
{
    this->shapeNumberAmounts.insert(Event::RED_CIRCLE, redCircles);
    this->shapeNumberAmounts.insert(Event::GREEN_SQUARE, greenSquares);
    this->shapeNumberAmounts.insert(Event::BLUE_HEXAGON, blueHexagons);
}

QString EpidemicEvent::getEventName() const
{
    return "EPIDEMIC";
}

Instruction *EpidemicEvent::happen(Instruction *next) const
{
    return new EpidemicEventInstruction(this->boardModel, next, this);
}
