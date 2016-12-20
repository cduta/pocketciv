#ifndef EPIDEMICEVENTINSTRUCTION_HPP
#define EPIDEMICEVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"

class EpidemicEventInstruction : public EventInstruction
{
    int step;

    int populationLoss;
    int oldTribes;

    //const EventCard *first;

public:
    EpidemicEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private:
    // Preperation steps.
    Instruction *setActiveRegion();
    Instruction *checkActiveRegion();
    Instruction *initializePopulationLoss();
    Instruction *continueEpidemic();
};

#endif // EPIDEMICEVENTINSTRUCTION_HPP
