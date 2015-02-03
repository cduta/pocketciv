#ifndef CIVILWAREVENT_HPP
#define CIVILWAREVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class CivilWarEvent : public Event
{
    BoardModel *boardModel;

public:
    CivilWarEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // CIVILWAREVENT_HPP
