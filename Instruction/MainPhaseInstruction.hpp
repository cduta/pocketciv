#ifndef MAINPHASEINSTRUCTION_HPP
#define MAINPHASEINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class MainPhaseInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    MainPhaseInstruction(BoardModel *boardModel);

    virtual void initInstruction() const;
};

#endif // MAINPHASEINSTRUCTION_HPP
