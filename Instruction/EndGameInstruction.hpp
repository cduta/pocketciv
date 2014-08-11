#ifndef ENDGAMEINSTRUCTION_HPP
#define ENDGAMEINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class EndGameInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    EndGameInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // ENDGAMEINSTRUCTION_HPP
