#ifndef DECIMATEGOLDINSTRUCTION_H
#define DECIMATEGOLDINSTRUCTION_H

#include <Instruction/Instruction.hpp>
#include "BoardModel.hpp"

class DecimateGoldInstruction : public Instruction
{
    BoardModel *boardModel;
    bool anarchy;

public:
    DecimateGoldInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // DECIMATEGOLDINSTRUCTION_H
