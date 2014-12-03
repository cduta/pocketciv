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
    Event(int redCircles = 0, int greenSquares = 0, int blueHexagons = 0);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;

    QMap<NumberShapes, int> getShapeNumberAmounts() const;
};

#endif //EVENT_HPP
