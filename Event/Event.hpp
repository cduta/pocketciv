#ifndef EVENT_HPP
#define EVENT_HPP

#include <QString>

#include "Instruction/Instruction.hpp"

class Event
{
public:
    virtual QString getEventName() const;
    virtual Instruction *happen(Instruction *current) const;
};

#endif //EVENT_HPP
