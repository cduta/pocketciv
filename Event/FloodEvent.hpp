#ifndef FLOODEVENT_HPP
#define FLOODEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class FloodEvent : public Event
{
    BoardModel *boardModel;

public:
    FloodEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // FLOODEVENT_HPP
