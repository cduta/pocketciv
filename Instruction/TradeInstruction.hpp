#ifndef TRADEINSTRUCTION_H
#define TRADEINSTRUCTION_H

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class TradeInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    QString what;

    int step;
    int goldGain;

public:
    TradeInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // TRADEINSTRUCTION_H
