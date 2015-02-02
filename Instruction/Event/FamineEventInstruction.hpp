#ifndef FAMINEEVENTINSTRUCTION_HPP
#define FAMINEEVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"

class FamineEventInstruction : public EventInstruction
{
    int step;

public:
    FamineEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // FAMINEEVENTINSTRUCTION_HPP
