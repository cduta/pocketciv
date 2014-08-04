#ifndef INITIALFORESTINSTRUCTION_HPP
#define INITIALFORESTINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"

class InitialForestInstruction : public Instruction
{

public:
    InitialForestInstruction(BoardModel *boardModel);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // INITIALFORESTINSTRUCTION_HPP
