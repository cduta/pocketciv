#ifndef EARTHQUAKEEVENT_HPP
#define EARTHQUAKEEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class EarthquakeEvent : public Event
{
    BoardModel *boardModel;

public:
    EarthquakeEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // EARTHQUAKEEVENT_HPP
