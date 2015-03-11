#ifndef COLLECTTAXESINSTRUCTION_H
#define COLLECTTAXESINSTRUCTION_H

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class CollectTaxesInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    bool done;
    int totalGoldGained;
    int nuggets;
    bool handshake;
    bool draw;
    const EventCard *card;

public:
    CollectTaxesInstruction(BoardModel *boardModel, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // COLLECTTAXESINSTRUCTION_H
