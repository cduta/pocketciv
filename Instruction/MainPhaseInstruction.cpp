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
    this->boardModel->sendMessage("MAIN PHASE:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("You can do any amount of actions by clicking on any of the buttons.");
    this->boardModel->sendMessage("After applying any actions to a region, at least 1 tribe has to remain.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are Done, press Done.");
    this->boardModel->sendMessage(" ");
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
    // TODO: Do not crash when canceled.
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
