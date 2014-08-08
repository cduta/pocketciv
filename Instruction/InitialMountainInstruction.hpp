#ifndef INITIALMOUNTAININSTRUCTION_HPP
#define INITIALMOUNTAININSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class InitialMountainInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    InitialMountainInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // INITIALMOUNTAININSTRUCTION_H
