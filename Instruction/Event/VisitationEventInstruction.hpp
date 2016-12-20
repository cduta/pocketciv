#ifndef VISITATIONEVENTINSTRUCTION_H
#define VISITATIONEVENTINSTRUCTION_H

#include "Instruction/Event/EventInstruction.hpp"
#include "DiplomaticOfferDialog.hpp"

class VisitationEventInstruction : public EventInstruction
{
    const BoardModel::Empire empire;

    int step;
    bool isTrading;
    int attackingForce;
    DiplomaticOfferDialog diplomaticOfferDialog;

public:
    VisitationEventInstruction(BoardModel::Empire empire, BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // VISITATIONEVENTINSTRUCTION_H
