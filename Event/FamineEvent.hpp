#ifndef FAMINEEVENT_HPP
#define FAMINEEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class FamineEvent : public Event
{
    BoardModel *boardModel;

public:
    FamineEvent(BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // FAMINEEVENT_HPP
