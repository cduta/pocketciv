#include "VisitationEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Instruction/TradeInstruction.hpp"
#include "Instruction/AttackInstruction.hpp"

VisitationEventInstruction::VisitationEventInstruction(BoardModel::Empire empire, BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), empire(empire), step(0), isTrading(false), attackingForce(0)
{}

void VisitationEventInstruction::initInstruction()
{
    this->boardModel->sendMessage(QString("VISITATION: %1").arg(this->boardModel->getEmpireName(this->empire)));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
    return;
}

Instruction *VisitationEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step++;
        this->isTrading = this->boardModel->drawCard()->hasHandshake();

        if(this->isTrading)
        {
            this->boardModel->sendMessage(QString("The %1 empire came to trade with you.").arg(this->boardModel->getEmpireName(this->empire)));
        }
        else
        {
            this->boardModel->sendMessage(QString("The %1 empire came with their military forces to visit your empire.").arg(this->boardModel->getEmpireName(this->empire)));
        }

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step++;
        if(this->isTrading)
        {
            Instruction *next =
                    new TradeInstruction(
                        this->boardModel,
                        this->nextInstruction,
                        this->boardModel->getEmpireName(this->empire));
            next->initInstruction();
            return next;
        }

        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 2)
    {
        this->step++;
        if(!this->boardModel->bordersOnSea(this->boardModel->refActiveRegion()->getRegion()) &&
           !this->boardModel->bordersOnFrontier(this->boardModel->refActiveRegion()->getRegion()))
        {
            this->boardModel->sendMessage(QString("The active region does NOT border on a sea or frontier,"));
            this->boardModel->sendMessage(QString("which means they could not have landed there."));
            this->boardModel->sendMessage(QString("Therefore, the %1 empire retreated again.").arg(this->boardModel->getEmpireName(this->empire)));
            this->boardModel->unsetActiveRegion();
            return this->endEvent();
        }
        else
        {
            this->boardModel->sendMessage(QString("The active region borders on a sea or frontier,"));
            this->boardModel->sendMessage(QString("which means the military forces landed there."));
            this->boardModel->sendMessage(QString("The %1 empire are here to pillage your empire.").arg(this->boardModel->getEmpireName(this->empire)));
        }

        this->attackingForce = this->boardModel->refOriginalCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 3)
    {
        this->step++;
        Instruction *next =
                new AttackInstruction(
                    this->boardModel,
                    this->nextInstruction,
                    this->boardModel->getEmpireName(this->empire),
                    this->attackingForce);
        next->initInstruction();
        return next;
    }

    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
