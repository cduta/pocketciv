#ifndef CORRUPTIONEVENT_HPP
#define CORRUPTIONEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class CorruptionEvent : public Event
{
    BoardModel *boardModel;

public:
    CorruptionEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // CORRUPTIONEVENT_HPP
