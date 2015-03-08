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
    this->boardModel->sendMessage("END OF ERA");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue...");
    return;
}

Instruction *EndOfEraInstruction::triggerDone()
{
    if(!this->advancesChosen)
    {
        this->advancesChosen = true;

        this->boardModel->sendMessage(" ");
        if(((this->boardModel->getEra() < this->boardModel->getLastEra() &&
            this->boardModel->getCityCount() >= this->boardModel->getEra()) ||
            this->boardModel->getEra() == this->boardModel->getLastEra()) &&
            this->boardModel->getTribeCount() > 0)
        {
            this->boardModel->sendMessage("Choose advances up to the amount of tribes in the empire to count towards your Glory Score.");
            this->boardModel->sendMessage(" ");

            this->boardModel->setDoneButton(false);
            connect(&this->advancesDialog, SIGNAL(finished(int)), this, SLOT(doneSelectingAdvances()));
            this->advancesDialog.updateDialog();
            this->advancesDialog.show();
        }
        else if(this->boardModel->getEra() < this->boardModel->getLastEra() &&
                this->boardModel->getCityCount() < this->boardModel->getEra())
        {
            this->boardModel->sendMessage("You can't choose any advance in this end of era.");
            this->boardModel->sendMessage("The amount of cities in the empire is less than the era about to end.");
            this->boardModel->sendMessage(QString("Amount of Cities: %1").arg(this->boardModel->getCityCount()));
            this->boardModel->sendMessage(QString("The current Era:  %1").arg(this->boardModel->getEra()));
            this->boardModel->sendMessage(" ");
        }
        else if(this->boardModel->getTribeCount() == 0)
        {
            this->boardModel->sendMessage("You can't choose any advance in this end of era.");
            this->boardModel->sendMessage("You have no tribes in the empire.");
            this->boardModel->sendMessage(" ");
        }
    }

    if(this->boardModel->getEra() == this->boardModel->getLastEra())
    {
        this->boardModel->sendMessage("The last era is over.");
    }
    else
    {
        DecisionDialog decisionDialog("End Game?", "Do you want to end the game and receive your final score?", "Yes", "No", true);
        int result = decisionDialog.exec();
        if(result == QDialog::Accepted)
        {
            this->endGame = true;
            this->boardModel->sendMessage("You chose to end the game.");
        }
    }

    if(this->boardModel->getEra() == this->boardModel->getLastEra() || this->endGame)
    {
        this->boardModel->sendMessage(" ");

        if(this->boardModel->getEra() == this->boardModel->getLastEra())
        {
            this->interruptedInstruction->deleteLater();
            Instruction *next = new EndGameInstruction(this->boardModel);
            next->initInstruction();
            return next;
        }
        else
        {
            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage(QString("Your final Glory Score is: %1 !").arg(this->boardModel->getGloryScore()));
            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage("THE GAME IS OVER.");
            this->boardModel->disableButtons();
            return new NoInstruction();
        }
    }
    else
    {
        this->boardModel->sendMessage("The era increases by 1.");
        this->boardModel->increaseEra();
        this->boardModel->sendMessage("Reshuffle cards...");
        this->boardModel->reshuffleCards();
        this->boardModel->sendMessage("Discarding 3 event cards...");
        this->boardModel->drawCard(false);
        this->boardModel->drawCard(false);
        this->boardModel->drawCard(false);
    }

    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("END OF ERA DONE");
    this->boardModel->sendMessage(" ");
    this->interruptedInstruction->setKeepInstruction(false);
    return this->interruptedInstruction;
}

void EndOfEraInstruction::doneSelectingAdvances()
{
    this->boardModel->setDoneButton(true);
    this->boardModel->sendMessage(QString("After selecting advances, your Glory Score is now %1.").arg(this->boardModel->getGloryScore()));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to Continue...");
    this->boardModel->sendMessage(" ");
}

