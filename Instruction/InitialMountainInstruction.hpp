#ifndef INITIALMOUNTAININSTRUCTION_HPP
#define INITIALMOUNTAININSTRUCTION_HPP

#include "Instruction/Instruction.hpp"

class InitialMountainInstruction : public Instruction
{
    Q_OBJECT

public:
    InitialMountainInstruction(BoardModel *boardModel, QObject *parent = 0);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // INITIALMOUNTAININSTRUCTION_H
