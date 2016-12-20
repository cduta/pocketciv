#ifndef MOVETRIBESINSTRUCTION_HPP
#define MOVETRIBESINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"
#include "HowManyDialog.hpp"

class MoveTribesInstruction : public Instruction
{
    BoardModel *boardModel;
    MoveTribeDialog *moveTribeDialog;

public:
    MoveTribesInstruction(BoardModel *boardModel);
    ~MoveTribesInstruction();

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // MOVETRIBESINSTRUCTION_HPP
