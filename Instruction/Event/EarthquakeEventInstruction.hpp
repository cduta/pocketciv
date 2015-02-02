#ifndef EARTHQUAKEEVENTINSTRUCTION_HPP
#define EARTHQUAKEEVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"

class EarthquakeEventInstruction : public EventInstruction
{
    int step;
    int faultLinesLeft;
    int populationLoss;

    QMap<int, RegionModel *> borderingRegions;
    QMap<int, RegionModel *> faultLinelessRegionModels;
    QMap<int, RegionModel *> tribeLossRegionModels;

public:
    EarthquakeEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // EARTHQUAKEEVENTINSTRUCTION_HPP
