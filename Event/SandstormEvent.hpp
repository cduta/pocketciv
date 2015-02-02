#ifndef SANDSTORMEVENT_HPP
#define SANDSTORMEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class SandstormEvent : public Event
{
    BoardModel *boardModel;

public:
    SandstormEvent(BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // SANDSTORMEVENT_HPP
