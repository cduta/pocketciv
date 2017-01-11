#include "VisitationEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Instruction/TradeInstruction.hpp"
#include "Instruction/AttackInstruction.hpp"
#include "Instruction/DialogInstruction.hpp"

#include "Common.hpp"

VisitationEventInstruction::VisitationEventInstruction(BoardModel::Empire empire, BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), empire(empire), step(0),
                       isTrading(false), attackingForce(0), diplomaticOfferDialog(this->boardModel)
{
}

void VisitationEventInstruction::initInstruction()
{
    QString affiliationString = "";

    if(this->boardModel->isTradingPartner(this->empire))
    {
        affiliationString = " (Trading Partner)";
    }

    this->boardModel->printMessage(QString("VISITATION: %1%2").arg(Common::getEmpireName(this->empire)).arg(affiliationString));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *VisitationEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel && this->step == 1)
    {
        if(button == Qt::LeftButton && this->boardModel->getTribeCount() - this->boardModel->getAllSelectedTribes() > 1 && regionModel->getSelectedTribes() < regionModel->getTribes())
        {
            regionModel->setSelectedTribes(regionModel->getSelectedTribes() + 1);
            this->diplomaticOfferDialog.updateTribeOfferLabel();
        }

        if(button == Qt::RightButton && regionModel->getSelectedTribes() > 0)
        {
            regionModel->setSelectedTribes(regionModel->getSelectedTribes() - 1);
            this->diplomaticOfferDialog.updateTribeOfferLabel();
        }
    }

    return this;
}

Instruction *VisitationEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step++;

        if(!this->boardModel->isTradingPartner(this->empire) && this->boardModel->hasAdvanceAquired(AdvanceModel::DIPLOMACY))
        {
            this->boardModel->printMessage("Advance (DIPLOMACY):");
            this->boardModel->printMessage("Before VISITATION begins, you may now attempt to");
            this->boardModel->printMessage("make the visiting Empire a Trading Partner by.");
            this->boardModel->printMessage("offering GOLD or TRIBES.");
            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("Trading Partner always trade whenever they visit and never attack.");
            this->boardModel->printMessage(" ");
            return new DialogInstruction(
                        this->boardModel,
                        this,
                        &(this->diplomaticOfferDialog));
        }

        this->step++;
    }

    if(this->step == 1)
    {
        this->step++;

        if(this->diplomaticOfferDialog.isSelectingTribes())
        {
            this->step = 1;
            this->diplomaticOfferDialog.unsetSelectedTrbes();
            return new DialogInstruction(
                        this->boardModel,
                        this,
                        &(this->diplomaticOfferDialog));
        }
    }

    if(this->step == 2)
    {
        this->step++;

        int diplomaticOfferNeeded = this->boardModel->drawCard()->getShapeNumbers().value(Event::RED_CIRCLE, 0);

        this->diplomaticOfferDialog.decimateOffer();
        if(this->diplomaticOfferDialog.getDiplomaticOffer() >= diplomaticOfferNeeded)
        {
            this->boardModel->setTradingPartner(this->empire);
            this->boardModel->printMessage("Your offering was sent and they accept.");
            this->boardModel->printMessage("The " +  Common::getEmpireName(this->empire) + " have now become a Trading Partner.");
            this->boardModel->printMessage(" ");
        }
        else
        {
            this->boardModel->setTradingPartner(this->empire);
            this->boardModel->printMessage("Your offering was sent but they decline.");
            this->boardModel->printMessage(" ");
        }

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 3)
    {
        this->step++;

        this->isTrading = this->boardModel->isTradingPartner(this->empire);

        if(this->isTrading)
        {
            this->boardModel->printMessage(QString("You and the %1 empire are trading partners.").arg(Common::getEmpireName(this->empire)));
        }
        else
        {
            this->boardModel->printMessage("You try negotiating for a trade, by drawing a card with a handshake...");
            this->isTrading = this->boardModel->drawCard()->hasHandshake();
        }

        if(this->isTrading)
        {
            this->boardModel->printMessage(QString("The %1 empire came to trade with you.").arg(Common::getEmpireName(this->empire)));
        }
        else
        {
            this->boardModel->printMessage(QString("But the %1 empire came with their military forces to visit your empire.").arg(Common::getEmpireName(this->empire)));
        }
        this->boardModel->printMessage(" ");

        if(!this->boardModel->hasAdvanceAquired(AdvanceModel::PHILOSOPHY) || this->isTrading)
        {
            this->step++;
        }

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 4)
    {
        this->step++;

        this->boardModel->printMessage("Advance (PHILOSOPHY):");
        this->boardModel->printMessage("You MAY draw a second card to try and negotiate one more time.");

        int result = QMessageBox::question(NULL, "Advance (PHILOSOPHY)", "You MAY draw a second card to try and negotiate one more time.\n\nDo you want to try one more time?", "Yes","No");

        if(result == 0) // YES
        {
            this->boardModel->printMessage("You try negotiating for a trade again...");
            this->isTrading = this->boardModel->drawCard()->hasHandshake();

            if(this->isTrading)
            {
                this->boardModel->printMessage(QString("The %1 empire came to trade with you.").arg(Common::getEmpireName(this->empire)));
            }
            else
            {
                this->boardModel->printMessage(QString("But they decline and their military forces draw closer.").arg(Common::getEmpireName(this->empire)));
            }

            this->boardModel->printMessage(" ");
            POCKET_CIV_END_OF_ERA_CHECK
        }
        else // NO
        {
            this->boardModel->printMessage("You did not try to negotiate for a trade again.");
            this->boardModel->printMessage(QString("The military forces of %1 empire draw closer.").arg(Common::getEmpireName(this->empire)));
        }
    }

    if(this->step == 5)
    {
        this->step++;
        if(this->isTrading)
        {
            Instruction *next =
                    new TradeInstruction(
                        this->boardModel,
                        this->nextInstruction,
                        Common::getEmpireName(this->empire));
            next->initInstruction();
            return next;
        }


        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 6)
    {
        this->step++;
        if(!this->boardModel->bordersOnSea(this->boardModel->refActiveRegion()->getRegion()) &&
           !this->boardModel->bordersOnFrontier(this->boardModel->refActiveRegion()->getRegion()))
        {
            this->boardModel->printMessage(QString("The active region does NOT border on a sea or frontier,"));
            this->boardModel->printMessage(QString("which means they could not have landed there."));
            this->boardModel->printMessage(QString("Therefore, the %1 empire retreated again.").arg(Common::getEmpireName(this->empire)));
            this->boardModel->printMessage(" ");
            this->boardModel->unsetActiveRegion();
            return this->endEvent();
        }
        else
        {
            this->boardModel->printMessage(QString("The active region borders on a sea or frontier,"));
            this->boardModel->printMessage(QString("which means the military forces landed there."));
            this->boardModel->printMessage(QString("The %1 empire are here to pillage your empire.").arg(Common::getEmpireName(this->empire)));
            this->boardModel->printMessage(" ");
        }

        this->attackingForce = this->boardModel->refOriginalCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 7)
    {
        this->step++;
        Instruction *next =
                new AttackInstruction(
                    this->boardModel,
                    this->nextInstruction,
                    Common::getEmpireName(this->empire),
                    this->attackingForce);
        next->initInstruction();
        return next;
    }

    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
