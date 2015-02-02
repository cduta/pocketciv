#include "BanditsEventInstruction.hpp"

#include "Instruction/AttackInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

BanditsEventInstruction::BanditsEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), attackingForce(0), step(0)
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
        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
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

            POCKET_CIV_END_OF_ERA_CHECK
        }
        else
        {
            this->boardModel->sendMessage(QString("The active region has no adjacent desert.").arg(this->attackingForce));
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

    return this->endEvent();
}
