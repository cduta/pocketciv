#ifndef CHOOSEFRONTIERINSTRUCTION_HPP
#define CHOOSEFRONTIERINSTRUCTION_HPP

#include "Instruction.hpp"

class ChooseFrontierInstruction : public Instruction
{
public:
    ChooseFrontierInstruction(BoardModel *boardModel);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // CHOOSEFRONTIERINSTRUCTION_HPP
