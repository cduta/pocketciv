#include "SuperstitionEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "DecisionDialog.hpp"

SuperstitionEventInstruction::SuperstitionEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0), discardsLeft(0), discardsTotal(0)
{}

void SuperstitionEventInstruction::initInstruction()
{
    this->boardModel->printMessage("SUPERSTITION:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *SuperstitionEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;

        QMap<Event::NumberShapes, int> shapeNumberAmounts = this->event->getShapeNumberAmounts();
        this->discardsLeft = this->boardModel->drawCard()->getShapeNumberSum(shapeNumberAmounts);

        if(this->boardModel->getAdvancesAquired().contains(AdvanceModel::ASTRONOMY))
        {
            this->discardsLeft += 2*shapeNumberAmounts[Event::GREEN_SQUARE];

            this->boardModel->printMessage("Advance (ASTRONOMY):");
            this->boardModel->printMessage("For each GREEN SQUARE, add 2 to the discards.");
            this->boardModel->printMessage(QString("There are %1 GREEN SQUARES.").arg(shapeNumberAmounts[Event::GREEN_SQUARE]));
            this->boardModel->printMessage(" ");
        }

        this->discardsTotal = this->discardsLeft;

        this->boardModel->printMessage(QString("Superstition makes you discard %1 cards.")
                                      .arg(this->discardsTotal));
        this->boardModel->printMessage("The end of era event still applies.");
        this->boardModel->printMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        while(this->discardsLeft > 0)
        {
            if(this->boardModel->hasAdvanceAquired(AdvanceModel::MEDITATION) && this->boardModel->getEventCardCount() == 1)
            {
                DecisionDialog decisionDialog("Stop SUPERSTITION?", "Advance (MEDITATION):\nIf only 1 event card to be drawn,\nyou can decide to stop SUPERSTITION.\n\nDo you want to stop SUPERSTITION?", "Yes", "No", true);
                int result = decisionDialog.exec();
                if(result == QDialog::Accepted)
                {
                    this->boardModel->printMessage(QString("You stopped the event when %1 out of %2 cards were already discarded.")
                                                   .arg(this->discardsTotal-this->discardsLeft)
                                                   .arg(this->discardsTotal));
                    this->boardModel->printMessage(" ");
                    return this->endEvent();
                }
            }

            this->discardsLeft--;
            this->boardModel->drawCard(false);

            if(this->boardModel->isEndOfEra())
            {
                if(this->discardsLeft > 0)
                {
                    this->boardModel->printMessage(QString("Cards discarded so far: %1 out of %2")
                                                  .arg(this->discardsTotal-this->discardsLeft)
                                                  .arg(this->discardsTotal));
                    this->boardModel->printMessage(" ");
                }
                else
                {
                    this->step = -1;
                    this->boardModel->printMessage(QString("Done discarding %1 cards.").arg(this->discardsTotal));
                    this->boardModel->printMessage(" ");
                }

                Instruction *next = new EndOfEraInstruction(this->boardModel, this);
                next->initInstruction();
                return next;
            }
        }

        this->boardModel->printMessage(QString("Done discarding %1 cards.").arg(this->discardsTotal));
        this->boardModel->printMessage(" ");
    }

    return this->endEvent();
}
