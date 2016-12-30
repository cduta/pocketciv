#ifndef UPRISINGEVENTINSTRUCTION_H
#define UPRISINGEVENTINSTRUCTION_H

#include "Instruction/Event/EventInstruction.hpp"

class UprisingEventInstruction : public EventInstruction
{
    int step;
    QMap<int, RegionModel *> possibleRegions;

public:
    UprisingEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};
#endif // UPRISINGEVENTINSTRUCTION_H
