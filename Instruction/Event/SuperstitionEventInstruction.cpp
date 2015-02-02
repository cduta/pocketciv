#include "SuperstitionEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

SuperstitionEventInstruction::SuperstitionEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0), discardsLeft(0), discardsTotal(0)
{}

void SuperstitionEventInstruction::initInstruction()
{
    this->boardModel->sendMessage("SUPERSTITION:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
    return;
}

Instruction *SuperstitionEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;

        this->discardsLeft = this->boardModel->drawCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
        this->discardsTotal = this->discardsLeft;

        this->boardModel->sendMessage(QString("Superstition makes you discard %1 cards.")
                                      .arg(this->discardsTotal));
        this->boardModel->sendMessage("The end of era event still applies.");
        this->boardModel->sendMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        while(this->discardsLeft > 0)
        {
            this->discardsLeft--;
            this->boardModel->drawCard(false);

            if(this->boardModel->isEndOfEra())
            {
                if(this->discardsLeft > 0)
                {
                    this->boardModel->sendMessage(QString("Cards discarded so far: %1 out of %2")
                                                  .arg(this->discardsTotal-this->discardsLeft)
                                                  .arg(this->discardsTotal));
                    this->boardModel->sendMessage(" ");
                }
                else
                {
                    this->step = -1;
                    this->boardModel->sendMessage(QString("Done discarding %1 cards.").arg(this->discardsTotal));
                    this->boardModel->sendMessage(" ");
                }

                Instruction *next = new EndOfEraInstruction(this->boardModel, this);
                next->initInstruction();
                return next;
            }
        }

        this->boardModel->sendMessage(QString("Done discarding %1 cards.").arg(this->discardsTotal));
        this->boardModel->sendMessage(" ");
    }

    return this->endEvent();
}
