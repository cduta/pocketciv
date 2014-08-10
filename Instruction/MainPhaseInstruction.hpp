#ifndef MAINPHASEINSTRUCTION_HPP
#define MAINPHASEINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class MainPhaseInstruction : public Instruction
{
    BoardModel *boardModel;
    bool initialized;

public:
    MainPhaseInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // MAINPHASEINSTRUCTION_HPP
