#ifndef INITIALMOUNTAININSTRUCTION_HPP
#define INITIALMOUNTAININSTRUCTION_HPP

#include "Instruction/Instruction.hpp"

class InitialMountainInstruction : public Instruction
{

public:
    InitialMountainInstruction(BoardModel *boardModel);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // INITIALMOUNTAININSTRUCTION_H
