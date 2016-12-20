#include "DialogInstruction.hpp"

DialogInstruction::DialogInstruction(BoardModel *boardModel, Instruction *interruptedInstruction, QDialog *instructionDialog, QObject *parent)
    : Instruction(parent), boardModel(boardModel), interruptedInstruction(interruptedInstruction), instructionDialog(instructionDialog)
{
    this->interruptedInstruction->setKeepInstruction(true);
    connect(this->instructionDialog, SIGNAL(accepted()), this, SLOT(closeDialog()));
    connect(this->instructionDialog, SIGNAL(rejected()), this, SLOT(closeDialog()));
    this->instructionDialog->show();
    this->boardModel->disableButtons();
}

Instruction *DialogInstruction::getFollowingInstruction()
{
    this->boardModel->setDoneButton(true);
    this->interruptedInstruction->setKeepInstruction(false);
    return this->interruptedInstruction;
}

void DialogInstruction::closeDialog()
{
    this->boardModel->emitDialogClosed();
    return;
}

