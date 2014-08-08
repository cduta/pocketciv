#ifndef DRAWEVENTCARDINSTRUCTION_HPP
#define DRAWEVENTCARDINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "EventCard.hpp"
#include "BoardModel.hpp"

class DrawEventCardInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *nextInstruction;
    const EventCard *originalEventCard;
    const Event *currentEvent;

public:
    DrawEventCardInstruction(BoardModel *boardModel, Instruction *nextInstruction, const EventCard *eventCard);

    virtual void initInstruction();
};

#endif // DRAWEVENTCARDINSTRUCTION_HPP
