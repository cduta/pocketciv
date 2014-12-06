#ifndef CORRUPTIONEVENTINSTRUCTION_HPP
#define CORRUPTIONEVENTINSTRUCTION_HPP

#include "Instruction/Event/EventInstruction.hpp"
#include "BoardModel.hpp"

class CorruptionEventInstruction : public EventInstruction
{
    int step;
    int corruption;
    int cityAVTotal;

public:
    CorruptionEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private:
    Instruction *endCorruptionEvent();
};

#endif // CORRUPTIONEVENTINSTRUCTION_HPP
