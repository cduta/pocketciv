#ifndef FLOODEVENTINSTRUCTION_HPP
#define FLOODEVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"

class FloodEventInstruction : public EventInstruction
{
    int step;

public:
    FloodEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // FLOODEVENTINSTRUCTION_HPP
