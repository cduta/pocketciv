#ifndef EVENTCARD_HPP
#define EVENTCARD_HPP

#include <QObject>
#include <QMap>

#include "Event/Event.hpp"

class EventCard : public QObject
{
    Q_OBJECT

    QMap<Event::NumberShapes, int> shapeNumbers;
    bool handshake;
    int goldNuggets;
    QMap<int, Event *> events;

public:
    EventCard(int redCircleNumber,
              int greenSquareNumber,
              int blueHexagonNumber,
              bool handshake,
              int goldNuggets,
              const QMap<int, Event *> &events,
              QObject *parent = 0);

    // Get-Methods
    const Event *getEvent(int era) const;
    QMap<Event::NumberShapes, int> getShapeNumbers() const;
    int getShapeNumberSum(const QMap<Event::NumberShapes, int> &shapeNumberAmounts) const;
    bool hasHandshake() const;
    int getGoldNuggets() const;
};

#endif // EVENTCARD_HPP
