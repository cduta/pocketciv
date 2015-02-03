#ifndef VISITATIONEVENT_H
#define VISITATIONEVENT_H

#include "Event.hpp"
#include "BoardModel.hpp"

class VisitationEvent : public Event
{
    BoardModel::Empire empire;
    BoardModel *boardModel;

public:
    VisitationEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel::Empire empire, BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // VISITATIONEVENT_H
