#ifndef BANDITSEVENTINSTRUCTION_HPP
#define BANDITSEVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"
#include "BoardModel.hpp"

class BanditsEventInstruction : public EventInstruction
{
    int attackingForce;
    int step;

public:
    BanditsEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // BANDITSEVENTINSTRUCTION_HPP
