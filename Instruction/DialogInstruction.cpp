#include "DialogInstruction.hpp"

DialogInstruction::DialogInstruction(BoardModel *boardModel, Instruction *previousInstruction, QDialog *instructionDialog, QObject *parent)
    : Instruction(parent), boardModel(boardModel), previousInstruction(previousInstruction), instructionDialog(instructionDialog)
{
    this->previousInstruction->setKeepInstruction(true);
    connect(this->instructionDialog, SIGNAL(accepted()), this, SLOT(closeDialog()));
    connect(this->instructionDialog, SIGNAL(rejected()), this, SLOT(closeDialog()));
    this->instructionDialog->show();
    this->boardModel->disableButtons();
}

DialogInstruction::~DialogInstruction()
{
    this->instructionDialog->deleteLater();
}

Instruction *DialogInstruction::getPreviousInstruction()
{
    this->boardModel->enableButtons();
    this->previousInstruction->setKeepInstruction(false);
    return this->previousInstruction;
}

void DialogInstruction::closeDialog()
{
    this->boardModel->sendDialogClosed();
    return;
}

