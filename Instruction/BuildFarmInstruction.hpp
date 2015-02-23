#ifndef BUILDFARMINSTRUCTION_H
#define BUILDFARMINSTRUCTION_H

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class BuildFarmInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;

public:
    BuildFarmInstruction(BoardModel *boardModel, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // BUILDFARMINSTRUCTION_H
