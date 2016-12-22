#ifndef FORESTATIONINSTRUCTION_HPP
#define FORESTATIONINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class ForestationInstruction: public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    int tribesCost;

public:
    ForestationInstruction(BoardModel *boardModel, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // FORESTATIONINSTRUCTION_HPP
