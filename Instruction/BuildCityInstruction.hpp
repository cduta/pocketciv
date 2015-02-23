#ifndef BUILDCITYINSTRUCTION_H
#define BUILDCITYINSTRUCTION_H

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class BuildCityInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;

public:
    BuildCityInstruction(BoardModel *boardModel, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // BUILDCITYINSTRUCTION_H
