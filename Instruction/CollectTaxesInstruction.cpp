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
    this->boardModel->printMessage("COLLECT TAXES:");
    this->boardModel->printMessage("Try collecting taxes.");
    this->boardModel->printMessage("Draw an Event Card, add the amount of Goldnuggets to your Gold.");
    this->boardModel->printMessage("If it shows a Handshake, draw again and decimate 1 Gold.\n"
                                  "If you are unable to decimate 1 Gold, this action ends and an ANARCHY Event starts.");
    this->boardModel->printMessage("If it does not show a Handshake, this action ends.");
    this->boardModel->printMessage(" ");

    this->done = QMessageBox::Yes !=
            QMessageBox::StandardButton(
                QMessageBox::question(NULL, "Collect Taxes", "Do you want to try collect taxes?",
                                      QMessageBox::Yes, QMessageBox::No));

    if(this->done)
    {
        this->boardModel->printMessage("Collect Taxes cancelled.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("Press Done to continue...");

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

            this->boardModel->printMessage(QString("You collected %1 Gold nuggets.").arg(this->nuggets));

            if(this->handshake)
            {
                this->boardModel->printMessage("There was a handshake, therefore draw another card and remove 1 Gold.");
                this->boardModel->printMessage(" ");
                this->boardModel->printMessage("Press Done to continue...");
            }
            else
            {
                this->boardModel->printMessage("There was no handshake.");
            }
            this->boardModel->printMessage(" ");

            POCKET_CIV_END_OF_ERA_CHECK
        }

        this->totalGoldGained += this->nuggets;
        this->boardModel->gainGold(this->nuggets);

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
                this->boardModel->printMessage("Could not remove 1 Gold, because there is no Gold left.");
                this->boardModel->printMessage("Anarchy reigns...");
                this->boardModel->printMessage(" ");

                EventInstruction *instruction = new AnarchyEventInstruction(this->boardModel, this->nextInstruction, NULL);
                instruction->initInstruction();
                return instruction;
            }
        }
    }

    this->boardModel->printMessage("Collecting Taxes is done.");
    this->boardModel->printMessage(" ");

    this->nextInstruction->setKeepInstruction(false);
    return this->nextInstruction;
}

