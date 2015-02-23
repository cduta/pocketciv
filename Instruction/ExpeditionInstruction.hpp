#ifndef EXPEDITIONINSTRUCTION_H
#define EXPEDITIONINSTRUCTION_H

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class ExpeditionInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    int gainGold;
    bool expeditionCardDrawn;

public:
    ExpeditionInstruction(BoardModel *boardModel, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // EXPEDITIONINSTRUCTION_H
