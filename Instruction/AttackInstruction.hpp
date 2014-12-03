#ifndef ATTACKINSTRUCTION_HPP
#define ATTACKINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class AttackInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    QString what;
    int attackingForce;
    QSet<RegionModel *> possibleRegions;

public:
    AttackInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what, int attackingForce);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // ATTACKINSTRUCTION_HPP
