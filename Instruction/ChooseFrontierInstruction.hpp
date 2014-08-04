#ifndef CHOOSEFRONTIERINSTRUCTION_HPP
#define CHOOSEFRONTIERINSTRUCTION_HPP

#include "Instruction.hpp"

class ChooseFrontierInstruction : public Instruction
{
    Q_OBJECT

public:
    ChooseFrontierInstruction(BoardModel *boardModel, QObject *parent = 0);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // CHOOSEFRONTIERINSTRUCTION_HPP
