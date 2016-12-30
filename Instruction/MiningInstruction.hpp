#ifndef MININGINSTRUCTION_HPP
#define MININGINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class MiningInstruction: public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    int tribesCost;
    int step;
    int mineCart;

public:
    MiningInstruction(BoardModel *boardModel, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // MININGINSTRUCTION_HPP
