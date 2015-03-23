#ifndef STEALINGINSTRUCTION_H
#define STEALINGINSTRUCTION_H

#include <QMessageBox>

#include <Instruction/Instruction.hpp>
#include "BoardModel.hpp"

class StealingInstruction : public Instruction
{
    Q_OBJECT

    BoardModel *boardModel;
    QString what;
    Instruction *nextInstruction;

    int step;
    int stash;
    bool done;

    QMessageBox decision;

public:
    StealingInstruction(BoardModel *boardModel, const QString &what, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerDone();

private slots:
    void decisionMade(QAbstractButton *button);
};

#endif // STEALINGINSTRUCTION_H
