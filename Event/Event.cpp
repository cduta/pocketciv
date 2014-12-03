#include "Event.hpp"

Event::Event(int redCircles, int greenSquares, int blueHexagons)
{
    this->shapeNumberAmounts.insert(Event::RED_CIRCLE, redCircles);
    this->shapeNumberAmounts.insert(Event::GREEN_SQUARE, greenSquares);
    this->shapeNumberAmounts.insert(Event::BLUE_HEXAGON, blueHexagons);
}

QString Event::getEventName() const
{
    return "GENERIC EVENT";
}

Instruction *Event::happen(Instruction *next) const
{
    return next;
}

QMap<Event::NumberShapes, int> Event::getShapeNumberAmounts() const
{
    return this->shapeNumberAmounts;
}
