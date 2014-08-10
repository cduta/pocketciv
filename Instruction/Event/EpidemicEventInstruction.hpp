#ifndef EPIDEMICEVENTINSTRUCTION_HPP
#define EPIDEMICEVENTINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class EpidemicEventInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    int step;

    int populationLoss;
    int oldTribes;

    const Event *event;
    const EventCard *originalCard;

    const EventCard *first;

public:
    EpidemicEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private:
    // Preperation steps.
    Instruction *drawFirstCard();
    Instruction *setActiveRegion();
    Instruction *initializePopulationLoss();
    Instruction *continueEpidemic();
};

#endif // EPIDEMICEVENTINSTRUCTION_HPP
