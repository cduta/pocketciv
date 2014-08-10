#ifndef EVENT_HPP
#define EVENT_HPP

#include <QString>
#include <QList>
#include <QMap>

#include "Instruction/Instruction.hpp"

class Event
{
public:
    enum NumberShapes {RED_CIRCLE = 0, GREEN_SQUARE, BLUE_HEXAGON};

protected:
    QMap<NumberShapes, int> shapeNumberAmounts;

public:
    Event(const QMap<NumberShapes, int> &shapeNumberAmounts = QMap<NumberShapes, int>());

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;

    QMap<NumberShapes, int> getShapeNumberAmounts() const;
};

#endif //EVENT_HPP
