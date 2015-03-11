#ifndef ADVANCECITYAVINSTRUCTION_H
#define ADVANCECITYAVINSTRUCTION_H

#include <Instruction/Instruction.hpp>
#include "BoardModel.hpp"

class AdvanceCityAVInstruction : public Instruction
{
    BoardModel *boardModel;
    int maximumCityAV;
    int toBePaid;

public:
    AdvanceCityAVInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // ADVANCECITYAVINSTRUCTION_H
