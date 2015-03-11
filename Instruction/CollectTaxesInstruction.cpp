#include "CollectTaxesInstruction.hpp"

#include <QMessageBox>

#include "Event/AnarchyEvent.hpp"
#include "Instruction/Event/AnarchyEventInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

CollectTaxesInstruction::CollectTaxesInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : boardModel(boardModel), nextInstruction(nextInstruction), totalGoldGained(0), nuggets(0), handshake(false), draw(true)
{
    this->nextInstruction->setKeepInstruction(true);
}

void CollectTaxesInstruction::initInstruction()
{
    this->boardModel->sendMessage("COLLECT TAXES:");
    this->boardModel->sendMessage("Try collecting taxes.");
    this->boardModel->sendMessage("Draw an Event Card, add the amount of Goldnuggets to your Gold.");
    this->boardModel->sendMessage("If it shows a Handshake, draw again and decimate 1 Gold.\n"
                                  "If you are unable to decimate 1 Gold, this action ends and an ANARCHY Event starts.");
    this->boardModel->sendMessage("If it does not show a Handshake, this action ends.");
    this->boardModel->sendMessage(" ");

    this->done = QMessageBox::Yes !=
            QMessageBox::StandardButton(
                QMessageBox::question(NULL, "Collect Taxes", "Do you want to try collect taxes?",
                                      QMessageBox::Yes, QMessageBox::No));

    if(this->done)
    {
        this->boardModel->sendMessage("Collect Taxes cancelled.");
        this->boardModel->sendMessage(" ");
    }

    this->boardModel->sendMessage("Press Done to continue...");

    return;
}

Instruction *CollectTaxesInstruction::triggerDone()
{
    if(!this->done)
    {
        if(this->draw)
        {
            this->card = this->boardModel->drawCard(true);
            this->draw = false;

            this->nuggets = this->card->getGoldNuggets();
            this->handshake = this->card->hasHandshake();

            this->boardModel->sendMessage(QString("You collected %1 Gold nuggets.").arg(this->nuggets));

            if(this->handshake)
            {
                this->boardModel->sendMessage("There was a handshake, therefore draw another card and remove 1 Gold.");
                this->boardModel->sendMessage(" ");
                this->boardModel->sendMessage("Press Done to continue...");
            }
            else
            {
                this->boardModel->sendMessage("There was no handshake.");
            }
            this->boardModel->sendMessage(" ");

            POCKET_CIV_END_OF_ERA_CHECK
        }

        this->totalGoldGained += this->nuggets;
        this->boardModel->addGold(this->nuggets);

        if(this->handshake)
        {
            if(this->boardModel->getGold() > 0)
            {
                this->draw = true;
                this->boardModel->removeGold(1);
                return this;
            }
            else
            {
                this->boardModel->sendMessage("Could not remove 1 Gold, because there is no Gold left.");
                this->boardModel->sendMessage("Anarchy reigns...");
                this->boardModel->sendMessage(" ");

                EventInstruction *instruction = new AnarchyEventInstruction(this->boardModel, this->nextInstruction, NULL);
                instruction->initInstruction();
                return instruction;
            }
        }
    }

    this->boardModel->sendMessage("Collecting Taxes is done.");
    this->boardModel->sendMessage(" ");

    this->nextInstruction->setKeepInstruction(false);
    return this->nextInstruction;
}

