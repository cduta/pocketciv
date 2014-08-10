#ifndef EVENTCARD_HPP
#define EVENTCARD_HPP

#include <QObject>
#include <QMap>

#include "Event/Event.hpp"

class EventCard : public QObject
{
    Q_OBJECT

    QMap<Event::NumberShapes, int> shapeNumbers;
    QMap<int, Event *> events;

public:
    EventCard(int redCircleNumber, int greenSquareNumber, int blueHexagonNumber, const QMap<int, Event *> &events, QObject *parent = 0);

    const Event *getEvent(int era) const;
    QMap<Event::NumberShapes, int> getShapeNumbers() const;
    int getShapeNumberSum(const QMap<Event::NumberShapes, int> &shapeNumberAmounts) const;
};

#endif // EVENTCARD_HPP
