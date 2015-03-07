#ifndef AQUIREADVANCEINSTRUCTION_H
#define AQUIREADVANCEINSTRUCTION_H

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"
#include "AdvancesDialog.hpp"

class AquireAdvanceInstruction : public Instruction
{
    Q_OBJECT

    BoardModel *boardModel;
    Instruction *nextInstruction;
    AdvancesDialog *advancesDialog;
    bool aquireAdvances;

public:
    AquireAdvanceInstruction(BoardModel *boardModel, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private slots:
    void doneAquiringAdvances();
};

#endif // AQUIREADVANCEINSTRUCTION_H
