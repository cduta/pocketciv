#ifndef VOLCANOEVENTINSTRUCTION_H
#define VOLCANOEVENTINSTRUCTION_H

#include "Instruction/Event/EventInstruction.hpp"

class VolcanoEventInstruction : public EventInstruction
{
    int step;
    QMap<int, RegionModel *> affectedRegions;

public:
    VolcanoEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // VOLCANOEVENTINSTRUCTION_H
