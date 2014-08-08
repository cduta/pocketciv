#ifndef EVENTCARD_HPP
#define EVENTCARD_HPP

#include <QObject>
#include <QMap>

#include "Event/Event.hpp"

class EventCard : public QObject
{
    Q_OBJECT

    QMap<int, Event *> events;

public:
    EventCard(const QMap<int, Event *> &events, QObject *parent = 0);

    const Event *getEvent(int era) const;
};

#endif // EVENTCARD_HPP
