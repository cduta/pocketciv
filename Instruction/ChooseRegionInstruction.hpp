#ifndef CHOOSEREGIONINSTRUCTION_HPP
#define CHOOSEREGIONINSTRUCTION_HPP

#include "Instruction.hpp"
#include "BoardModel.hpp"

class ChooseRegionInstruction : public Instruction
{
    BoardModel *boardModel;
    int currentRegion;
    int maxRegions;

public:
    ChooseRegionInstruction(BoardModel *boardModel, int currentRegion, int maxRegions);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // CHOOSEREGIONINSTRUCTION_HPP
