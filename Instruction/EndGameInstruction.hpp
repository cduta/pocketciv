#ifndef ENDGAMEINSTRUCTION_HPP
#define ENDGAMEINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class EndGameInstruction : public Instruction
{
    BoardModel *boardModel;
    bool countAdvances;

public:
    EndGameInstruction(BoardModel *boardModel, bool countAdvances = false);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // ENDGAMEINSTRUCTION_HPP
