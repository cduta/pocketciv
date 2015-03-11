#ifndef UPKEEPINSTRUCTION_HPP
#define UPKEEPINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class UpkeepInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    UpkeepInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerDone();

};

#endif // UPKEEPINSTRUCTION_HPP
