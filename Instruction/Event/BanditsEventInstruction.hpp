#ifndef BANDITSEVENTINSTRUCTION_HPP
#define BANDITSEVENTINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class BanditsEventInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    const Event *event;
    int attackingForce;
    int step;

public:
    BanditsEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // BANDITSEVENTINSTRUCTION_HPP
