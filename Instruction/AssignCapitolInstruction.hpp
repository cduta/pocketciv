#ifndef ASSIGNCAPITOLINSTRUCTION_H
#define ASSIGNCAPITOLINSTRUCTION_H


#include <Instruction/Instruction.hpp>
#include "BoardModel.hpp"

class AssignCapitolInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    AssignCapitolInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // ASSIGNCAPITOLINSTRUCTION_H
