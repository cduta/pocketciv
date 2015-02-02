#ifndef SUPERSTITIONEVENTINSTRUCTION_H
#define SUPERSTITIONEVENTINSTRUCTION_H

#include "Instruction/Event/EventInstruction.hpp"

class SuperstitionEventInstruction : public EventInstruction
{
    int step;
    int discardsLeft;
    int discardsTotal;

public:
    SuperstitionEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // SUPERSTITIONEVENTINSTRUCTION_H
