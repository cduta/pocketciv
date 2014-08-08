#ifndef INITIALFORESTINSTRUCTION_HPP
#define INITIALFORESTINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class InitialForestInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    InitialForestInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // INITIALFORESTINSTRUCTION_HPP
