#ifndef TSUNAMIINSTRUCTION_HPP
#define TSUNAMIINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "Event/Event.hpp"
#include "BoardModel.hpp"

class TsunamiInstruction : public Instruction
{
    int step;
    int tsunamiDamage;
    BoardModel *boardModel;
    RegionModel *activeRegion;
    Instruction *nextInstruction;
    const Event *firstEvent;
    QList<RegionModel *> affectedRegions;

public:
    TsunamiInstruction(BoardModel *boardModel, RegionModel *activeRegion, Instruction *nextInstruction, const Event *firstEvent);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // TSUNAMIINSTRUCTION_HPP
