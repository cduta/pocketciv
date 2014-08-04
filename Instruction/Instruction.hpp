#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <QObject>

#include "BoardModel.hpp"

class Instruction
{

protected:
    BoardModel *boardModel;

public:
    Instruction(BoardModel *boardModel);
    virtual ~Instruction();

    virtual Instruction *triggerHex(Qt::MouseButton, int, int);
    virtual Instruction *triggerDone();
};

class NoInstruction : public Instruction
{

public:
    NoInstruction(BoardModel *boardModel)
        : Instruction(boardModel)
    {}
};

#endif // INSTRUCTION_HPP
