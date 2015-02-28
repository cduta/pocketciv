#include "EndOfEraInstruction.hpp"

#include "DecisionDialog.hpp"
#include "Instruction/EndGameInstruction.hpp"

EndOfEraInstruction::EndOfEraInstruction(BoardModel *boardModel, Instruction *interruptedInstruction)
    : Instruction(), boardModel(boardModel), interruptedInstruction(interruptedInstruction), endGame(false)
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
    if(this->boardModel->getEra() < this->boardModel->getLastEra() &&
       this->boardModel->getCityCount() >= this->boardModel->getEra())
    {
       // TODO: Set this to: choose at most as many advances, as there are tribes in the empire.
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
            this->boardModel->sendMessage("The game is over.");
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

