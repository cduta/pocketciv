#ifndef BANDITSEVENT_HPP
#define BANDITSEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class BanditsEvent : public Event
{
    BoardModel *boardModel;

public:
    BanditsEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};
#endif // BANDITSEVENT_HPP
