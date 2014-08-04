#ifndef SETINITIALDESERTINSTRUCTION_HPP
#define SETINITIALDESERTINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"

class SetInitialDesertInstruction : public Instruction
{

public:
    SetInitialDesertInstruction(BoardModel *boardModel);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // SETINITIALDESERTINSTRUCTION_HPP
