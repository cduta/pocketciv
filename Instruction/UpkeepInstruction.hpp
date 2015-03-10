#ifndef UPKEEPINSTRUCTION_HPP
#define UPKEEPINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class UpkeepInstruction : public Instruction
{
    BoardModel *boardModel;
    bool done;
    int maximumCityAV;
    int toBePaid;

public:
    UpkeepInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private:
    void endTurn();
};

#endif // UPKEEPINSTRUCTION_HPP
