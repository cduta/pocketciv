#ifndef CHOOSEREGIONINSTRUCTION_HPP
#define CHOOSEREGIONINSTRUCTION_HPP

#include "Instruction.hpp"

class ChooseRegionInstruction : public Instruction
{
    int currentRegion;
    int maxRegions;

public:
    ChooseRegionInstruction(BoardModel *boardModel, int currentRegion, int maxRegions);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // CHOOSEREGIONINSTRUCTION_HPP
