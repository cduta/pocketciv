#include "Event.hpp"

Event::Event(const QMap<NumberShapes, int> &shapeNumberAmounts)
    : shapeNumberAmounts(shapeNumberAmounts)
{}

QString Event::getEventName() const
{
    return "Generic Event";
}

Instruction *Event::happen(Instruction *next) const
{
    return next;
}

QMap<Event::NumberShapes, int> Event::getShapeNumberAmounts() const
{
    return this->shapeNumberAmounts;
}
