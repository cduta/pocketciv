#ifndef MOVETRIBESINSTRUCTION_HPP
#define MOVETRIBESINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class MoveTribesInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    MoveTribesInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // MOVETRIBESINSTRUCTION_HPP
