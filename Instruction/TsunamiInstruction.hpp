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
    QList<RegionModel *> possibleAffectedRegions;

    QMap<int, RegionModel *> affectedRegions;

    int damagePerTribe;
    int damagePerCityAV;
    int damagePerWonder;

public:
    TsunamiInstruction(BoardModel *boardModel, RegionModel *activeRegion, Instruction *nextInstruction, const Event *firstEvent);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // TSUNAMIINSTRUCTION_HPP
