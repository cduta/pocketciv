#ifndef CHOOSEFRONTIERINSTRUCTION_HPP
#define CHOOSEFRONTIERINSTRUCTION_HPP

#include "Instruction.hpp"
#include "BoardModel.hpp"

class ChooseFrontierInstruction : public Instruction
{
    BoardModel *boardModel;

public:
    ChooseFrontierInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // CHOOSEFRONTIERINSTRUCTION_HPP
