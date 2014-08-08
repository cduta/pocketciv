#ifndef DIALOGINSTRUCTION_HPP
#define DIALOGINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class DialogInstruction : public Instruction
{
    Q_OBJECT

private:
    BoardModel *boardModel;
    Instruction *previousInstruction;
    QDialog *instructionDialog;

public:
    DialogInstruction(BoardModel *boardModel, Instruction *previousInstruction, QDialog *instructionDialog, QObject *parent = 0);
    ~DialogInstruction();

    virtual Instruction *getPreviousInstruction();

private slots:
    void closeDialog();
};

#endif // DIALOGINSTRUCTION_HPP
