#ifndef POPULATIONGROWTHINSTRUCTION_HPP
#define POPULATIONGROWTHINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class PopulationGrowthInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    PopulationGrowthInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // POPULATIONGROWTHINSTRUCTION_HPP
