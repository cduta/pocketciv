#include "MainPhaseInstruction.hpp"

#include "UpkeepInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

#include <QFileDialog>
#include <QMessageBox>

MainPhaseInstruction::MainPhaseInstruction(BoardModel *boardModel)
    : Instruction(),
      boardModel(boardModel),
      wonderDialogOpen(false),
      wonderDialog(NULL)
{}

void MainPhaseInstruction::initInstruction()
{
    this->boardModel->printMessage("MAIN PHASE:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Use the buttons to perform actions. After applying any action,");
    this->boardModel->printMessage("at least 1 tribe has to remain in the EMPIRE.");
    this->boardModel->printMessage("Note: To check Wonders built per region, right click on region.");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("When you are Done, press Done.");
    this->boardModel->printMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();
    this->wonderDialogOpen = false;
    return;
}

Instruction *MainPhaseInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel != NULL)
    {
        if(button == Qt::RightButton && !this->wonderDialogOpen)
        {
            this->wonderDialogOpen = true;
            this->wonderDialog = new WonderDialog(this->boardModel, WonderDescription::REGION_OVERVIEW, regionModel->getRegion());
            connect(this->wonderDialog, SIGNAL(finished(int)), this, SLOT(doneWonderDialogOverview()));
            this->wonderDialog->show();
        }
    }

    return this;
}

Instruction *MainPhaseInstruction::triggerDone()
{
    if(this->wonderDialogOpen)
    {
        return this;
    }

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

void MainPhaseInstruction::doneWonderDialogOverview()
{
    disconnect(this->wonderDialog, SIGNAL(finished(int)), this, SLOT(doneWonderDialogOverview()));

    this->wonderDialogOpen = false;

    this->wonderDialog->deleteLater();
}
