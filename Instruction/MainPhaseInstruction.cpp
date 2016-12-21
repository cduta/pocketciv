#include "MainPhaseInstruction.hpp"

#include "UpkeepInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

#include <QFileDialog>
#include <QMessageBox>

MainPhaseInstruction::MainPhaseInstruction(BoardModel *boardModel)
    : Instruction(),
      boardModel(boardModel)
{}

void MainPhaseInstruction::initInstruction()
{
    this->boardModel->printMessage("MAIN PHASE:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Use the buttons to perform actions. After applying any action,");
    this->boardModel->printMessage("at least 1 tribe has to remain in the EMPIRE.");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("When you are Done, press Done.");
    this->boardModel->printMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();
    return;
}

Instruction *MainPhaseInstruction::triggerDone()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(NULL , "Main Phase", "Do you really want to end the Main Phase?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No)
    {
        return this;
    }

    this->boardModel->disableButtons();
    this->boardModel->setDoneButton(true);
    Instruction *next = new UpkeepInstruction(this->boardModel);
    next->initInstruction();
    return next;
}

Instruction *MainPhaseInstruction::triggerSaveGame()
{
    // TODO: Autosave everytime this phase starts.
    QString saveFile =
    QFileDialog::getSaveFileName(NULL,
                                 "Save game",
                                 QDir::current().path(),
                                 "PocketCiv Saves (*.pcsave)");

    if(saveFile.isEmpty())
    {
        return this;
    }

    if(!saveFile.endsWith(".pcsave"))
    {
        saveFile.append(".pcsave");
    }

    QFile file(saveFile);
    file.open(QFile::WriteOnly);
    QDataStream writer(&file);

    this->boardModel->serialize(writer);

    return this;
}
