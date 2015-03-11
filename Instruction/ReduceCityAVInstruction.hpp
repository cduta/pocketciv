#ifndef REDUCECITYAVINSTRUCTION_H
#define REDUCECITYAVINSTRUCTION_H

#include <Instruction/Instruction.hpp>
#include "BoardModel.hpp"

class ReduceCityAVInstruction : public Instruction
{
    BoardModel *boardModel;
    int unsupportedCities;

public:
    ReduceCityAVInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // REDUCECITYAVINSTRUCTION_H
