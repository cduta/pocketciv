#ifndef MOVETRIBESINSTRUCTION_HPP
#define MOVETRIBESINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"
#include "HowManyDialog.hpp"

class MoveTribesInstruction : public Instruction
{
    BoardModel *boardModel;
    MoveTribeDialog *moveTribeDialog;
    const bool firstMovement;

public:
    MoveTribesInstruction(BoardModel *boardModel, bool firstMovement = true);
    ~MoveTribesInstruction();

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // MOVETRIBESINSTRUCTION_HPP
