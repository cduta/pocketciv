#ifndef TRADEINSTRUCTION_H
#define TRADEINSTRUCTION_H

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

#include <QMessageBox>

class TradeInstruction : public Instruction
{
    Q_OBJECT

    BoardModel *boardModel;
    Instruction *nextInstruction;
    QString what;

    int step;
    int goldGain;

    QMessageBox stealingDecision;

public:
    TradeInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what);

    virtual void initInstruction();

    virtual Instruction *triggerDone();

private slots:
    void stealingDecided(QAbstractButton *button);
};

#endif // TRADEINSTRUCTION_H
