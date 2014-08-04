#ifndef MOVETRIBESINSTRUCTION_HPP
#define MOVETRIBESINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"

class MoveTribesInstruction : public Instruction
{

public:
    MoveTribesInstruction(BoardModel *boardModel);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // MOVETRIBESINSTRUCTION_HPP
