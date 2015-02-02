#ifndef TRIBALWAREVENT_H
#define TRIBALWAREVENT_H

#include "Event.hpp"
#include "BoardModel.hpp"

class TribalWarEvent : public Event
{
    BoardModel *boardModel;

public:
    TribalWarEvent(BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};
#endif // TRIBALWAREVENT_H
