#ifndef SANDSTORMEVENTINSTRUCTION_HPP
#define SANDSTORMEVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"

class SandstormEventInstruction : public EventInstruction
{
    int step;
    int desertsLeft;
    QMap<int, RegionModel *> borderingRegions;

public:
    SandstormEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private:
    void applySandstorm(RegionModel *region);
};

#endif // SANDSTORMEVENTINSTRUCTION_HPP
