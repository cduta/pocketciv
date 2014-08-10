#ifndef EPIDEMICEVENT_HPP
#define EPIDEMICEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class EpidemicEvent : public Event
{
    BoardModel *boardModel;

public:
    EpidemicEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // EPIDEMICEVENT_HPP
