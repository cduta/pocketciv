#include "BanditsEventInstruction.hpp"

#include "Instruction/AttackInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

BanditsEventInstruction::BanditsEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : boardModel(boardModel), nextInstruction(nextInstruction), event(event), attackingForce(0), step(0)
{
    this->nextInstruction->setKeepInstruction(true);
}

void BanditsEventInstruction::initInstruction()
{
    this->boardModel->sendMessage("BANDITS:");
    this->boardModel->sendMessage("Press Done to continue.");

    return;
}

Instruction *BanditsEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;
        this->boardModel->setActiveRegion(this->boardModel->drawCard()->getShapeNumbers().value(Event::RED_CIRCLE, 0), true);
        this->boardModel->sendMessage(QString("The active region is %1.").arg(this->boardModel->refActiveRegion()->getRegion()));
        this->boardModel->sendMessage(" ");

        if(this->boardModel->isEndOfEra())
        {
            Instruction *next = new EndOfEraInstruction(this->boardModel, this);
            next->initInstruction();
            return next;
        }
    }

    if(this->step == 1)
    {
        this->step = 2;

        if(this->boardModel->refActiveRegion()->hasDesert() ||
           this->boardModel->bordersOnDesert(this->boardModel->refActiveRegion()->getRegion()))
        {
            this->attackingForce = this->boardModel->refOriginalCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
            this->boardModel->sendMessage(QString("The attacking force of the bandits is %1.").arg(this->attackingForce));
            this->boardModel->sendMessage(" ");

            if(this->boardModel->isEndOfEra())
            {
                Instruction *next = new EndOfEraInstruction(this->boardModel, this);
                next->initInstruction();
                return next;
            }
        }
        else
        {
            this->boardModel->sendMessage(QString("The active region has no adjacent desert.").arg(this->attackingForce));
            this->boardModel->sendMessage("The event ends...");
            this->boardModel->sendMessage(" ");
        }
    }

    if(this->step == 2 && this->attackingForce > 0)
    {
        Instruction *next =
                new AttackInstruction(
                    this->boardModel,
                    this->nextInstruction,
                    "BANDITS",
                    this->attackingForce);
        next->initInstruction();
        return next;
    }

    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
