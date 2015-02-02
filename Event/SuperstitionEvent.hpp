#ifndef SUPERSTITIONEVENT_H
#define SUPERSTITIONEVENT_H

#include "Event.hpp"
#include "BoardModel.hpp"

class SuperstitionEvent : public Event
{
    BoardModel *boardModel;

public:
    SuperstitionEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // SUPERSTITION_H
