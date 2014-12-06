#ifndef ANARCHYEVENTINSTRUCTION_HPP
#define ANARCHYEVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"
#include "BoardModel.hpp"

class AnarchyEventInstruction : public EventInstruction
{

public:
    AnarchyEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // ANARCHYEVENTINSTRUCTION_HPP
