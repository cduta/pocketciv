#ifndef ANARCHYEVENT_HPP
#define ANARCHYEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class AnarchyEvent : public Event
{
    BoardModel *boardModel;

public:
    AnarchyEvent(BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // ANARCHYEVENT_HPP
