#ifndef VOLCANOEVENT_H
#define VOLCANOEVENT_H

#include "Event.hpp"
#include "BoardModel.hpp"

class VolcanoEvent : public Event
{
    BoardModel *boardModel;

public:
    VolcanoEvent(BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *next) const;
};

#endif // VOLCANOEVENT_H
