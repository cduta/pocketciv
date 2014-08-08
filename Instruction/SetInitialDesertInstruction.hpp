#ifndef SETINITIALDESERTINSTRUCTION_HPP
#define SETINITIALDESERTINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class SetInitialDesertInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    SetInitialDesertInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // SETINITIALDESERTINSTRUCTION_HPP
