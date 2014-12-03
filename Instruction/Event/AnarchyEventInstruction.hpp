#ifndef ANARCHYEVENTINSTRUCTION_HPP
#define ANARCHYEVENTINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class AnarchyEventInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    const Event *event;

public:
    AnarchyEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // ANARCHYEVENTINSTRUCTION_HPP
