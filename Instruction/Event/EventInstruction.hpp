#ifndef EVENTINSTRUCTION_HPP
#define EVENTINSTRUCTION_HPP

#include <QObject>
#include <QDialog>

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class EventInstruction : public Instruction
{
    Q_OBJECT

protected:
    BoardModel *boardModel;
    Instruction *nextInstruction;
    const Event *event;

public:
    EventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event, QObject * parent = 0);

    void drawActiveRegion();
    Instruction *endEvent();
};

#endif // EVENTINSTRUCTION_HPP
