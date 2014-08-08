#ifndef EPIDEMICEVENT_HPP
#define EPIDEMICEVENT_HPP

#include "Event.hpp"
#include "BoardModel.hpp"

class EpidemicEvent : public Event
{
    BoardModel *boardModel;

public:
    EpidemicEvent(BoardModel *boardModel);

    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *current) const;
};

#endif // EPIDEMICEVENT_HPP
