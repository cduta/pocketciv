#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <QObject>

#include "BoardModel.hpp"

class Instruction : public QObject
{
    Q_OBJECT
protected:
    BoardModel *boardModel;

public:
    Instruction(BoardModel *boardModel, QObject *parent = 0);

    virtual Instruction *triggerHex(Qt::MouseButton, int, int);
    virtual Instruction *triggerDone();
};

class NoInstruction : public Instruction
{
    Q_OBJECT

public:
    NoInstruction(BoardModel *boardModel, QObject *parent)
        : Instruction(boardModel, parent)
    {}
};

#endif // INSTRUCTION_HPP
