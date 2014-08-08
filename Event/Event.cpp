#include "Event.hpp"

QString Event::getEventName() const
{
    return "Generic Event";
}

Instruction *Event::happen(Instruction *current) const
{
    return current;
}
