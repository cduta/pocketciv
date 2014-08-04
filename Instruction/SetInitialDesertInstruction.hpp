#ifndef SETINITIALDESERTINSTRUCTION_HPP
#define SETINITIALDESERTINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"

class SetInitialDesertInstruction : public Instruction
{
    Q_OBJECT

public:
    SetInitialDesertInstruction(BoardModel *boardModel, QObject *parent = 0);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // SETINITIALDESERTINSTRUCTION_HPP
