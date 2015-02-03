#ifndef VISITATIONEVENTINSTRUCTION_H
#define VISITATIONEVENTINSTRUCTION_H

#include "Instruction/Event/EventInstruction.hpp"

class VisitationEventInstruction : public EventInstruction
{
    const BoardModel::Empire empire;

    int step;
    bool isTrading;
    int attackingForce;

public:
    VisitationEventInstruction(BoardModel::Empire empire, BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // VISITATIONEVENTINSTRUCTION_H
