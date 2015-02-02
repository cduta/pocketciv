#ifndef TRIBALWAREVENTINSTRUCTION_H
#define TRIBALWAREVENTINSTRUCTION_H

#include "Instruction/Event/EventInstruction.hpp"

class TribalWarEventInstruction : public EventInstruction
{
    int step;
    int warringTribes;
    int borderingRegionsLeft;
    QMap<int, RegionModel *> borderingRegions;

public:
    TribalWarEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // TRIBALWAREVENTINSTRUCTION_H
