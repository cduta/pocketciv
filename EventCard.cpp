#include "EventCard.hpp"

EventCard::EventCard(const QMap<int, Event *> &events, QObject *parent)
    : QObject(parent), events(events)
{}

const Event *EventCard::getEvent(int era) const
{
    return this->events.value(era, NULL);
}
