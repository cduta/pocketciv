#ifndef UPRISINGEVENT_H
#define UPRISINGEVENT_H

#include "Event.hpp"
#include "BoardModel.hpp"

class UprisingEvent : public Event
{
    BoardModel *boardModel;

public:
    UprisingEvent(BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // UPRISINGEVENT_H
