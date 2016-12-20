#include "EndOfEraInstruction.hpp"

#include "DecisionDialog.hpp"
#include "Instruction/EndGameInstruction.hpp"

EndOfEraInstruction::EndOfEraInstruction(BoardModel *boardModel, Instruction *interruptedInstruction)
    : Instruction(), boardModel(boardModel), interruptedInstruction(interruptedInstruction), endGame(false), advancesChosen(false), advancesDialog(this->boardModel, AdvanceItem::SELECTABLE)
{
    this->interruptedInstruction->setKeepInstruction(true);
}

void EndOfEraInstruction::initInstruction()
{
    this->boardModel->printMessage("END OF ERA");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue...");
    return;
}

Instruction *EndOfEraInstruction::triggerDone()
{
    if(!this->advancesChosen)
    {
        this->advancesChosen = true;

        this->boardModel->printMessage(" ");
        if(((this->boardModel->getEra() < this->boardModel->getLastEra() &&
            this->boardModel->getCityCount() >= this->boardModel->getEra()) ||
            this->boardModel->getEra() == this->boardModel->getLastEra()) &&
            this->boardModel->getTribeCount() > 0)
        {
            this->boardModel->printMessage("Choose advances up to the amount of tribes in the empire to count towards your Glory Score.");
            this->boardModel->printMessage(" ");

            this->boardModel->setDoneButton(false);
            connect(&this->advancesDialog, SIGNAL(finished(int)), this, SLOT(doneSelectingAdvances()));
            this->advancesDialog.updateDialog();
            this->advancesDialog.show();
        }
        else if(this->boardModel->getEra() < this->boardModel->getLastEra() &&
                this->boardModel->getCityCount() < this->boardModel->getEra())
        {
            this->boardModel->printMessage("You can't choose any advance in this end of era.");
            this->boardModel->printMessage("The amount of cities in the empire is less than the era about to end.");
            this->boardModel->printMessage(QString("Amount of Cities: %1").arg(this->boardModel->getCityCount()));
            this->boardModel->printMessage(QString("The current Era:  %1").arg(this->boardModel->getEra()));
            this->boardModel->printMessage(" ");
        }
        else if(this->boardModel->getTribeCount() == 0)
        {
            this->boardModel->printMessage("You can't choose any advance in this end of era.");
            this->boardModel->printMessage("You have no tribes in the empire.");
            this->boardModel->printMessage(" ");
        }
    }

    if(this->boardModel->getEra() == this->boardModel->getLastEra())
    {
        this->boardModel->printMessage("The last era is over.");
    }
    else
    {
        DecisionDialog decisionDialog("End Game?", "Do you want to end the game and receive your final score?", "Yes", "No", true);
        int result = decisionDialog.exec();
        if(result == QDialog::Accepted)
        {
            this->endGame = true;
            this->boardModel->printMessage("You chose to end the game.");
        }
    }

    if(this->boardModel->getEra() == this->boardModel->getLastEra() || this->endGame)
    {
        this->boardModel->printMessage(" ");

        if(this->boardModel->getEra() == this->boardModel->getLastEra())
        {
            this->interruptedInstruction->deleteLater();
            Instruction *next = new EndGameInstruction(this->boardModel);
            next->initInstruction();
            return next;
        }
        else
        {
            this->boardModel->printMessage(" ");
            this->boardModel->printMessage(QString("Your final Glory Score is: %1 !").arg(this->boardModel->getGloryScore()));
            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("THE GAME IS OVER.");
            this->boardModel->disableButtons();
            return new NoInstruction();
        }
    }
    else
    {
        this->boardModel->printMessage("The era increases by 1.");
        this->boardModel->increaseEra();
        this->boardModel->printMessage("Reshuffle cards...");
        this->boardModel->reshuffleCards();
        this->boardModel->printMessage("Discarding 3 event cards...");
        this->boardModel->drawCard(false);
        this->boardModel->drawCard(false);
        this->boardModel->drawCard(false);
    }

    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("END OF ERA DONE");
    this->boardModel->printMessage(" ");
    this->interruptedInstruction->setKeepInstruction(false);
    return this->interruptedInstruction;
}

void EndOfEraInstruction::doneSelectingAdvances()
{
    this->boardModel->setDoneButton(true);
    this->boardModel->printMessage(QString("After selecting advances, your Glory Score is now %1.").arg(this->boardModel->getGloryScore()));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to Continue...");
    this->boardModel->printMessage(" ");
}

