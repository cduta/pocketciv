#include "TradeInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Instruction/StealingInstruction.hpp"

TradeInstruction::TradeInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), what(what), step(0), goldGain(0), handshake(false), additionalGoldGain(0)
{
    this->nextInstruction->setKeepInstruction(true);
}

void TradeInstruction::initInstruction()
{
    this->boardModel->printMessage(QString("TRADE with %1:").arg(this->what));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *TradeInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;
        const EventCard *card = this->boardModel->drawCard();

        this->goldGain = card->getGoldNuggets();
        this->handshake = card->hasHandshake();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step = 2;
        this->boardModel->printMessage(QString("While trading with %1, you gain %2 gold.").arg(this->what).arg(this->goldGain));
        this->boardModel->printMessage(" ");
        this->boardModel->gainGold(this->goldGain);
    }

    if(this->step == 2)
    {
        this->step = 3;

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::SHIPPING))
        {
            this->boardModel->printMessage("Advance (SHIPPING):");
            this->boardModel->printMessage("If the previously drawn card has a HANDSHAKE,");
            this->boardModel->printMessage("draw another card and add its GREEN SQUARE");
            this->boardModel->printMessage("number to the amount of gold you gain.");
            this->boardModel->printMessage(" ");

            if(this->handshake)
            {
                this->additionalGoldGain = this->boardModel->drawCard()->getShapeNumbers().value(Event::GREEN_SQUARE);
            }
            else
            {
                this->boardModel->printMessage("The previously drawn card had no HANDSHAKE.");
                this->boardModel->printMessage(" ");
            }

        }

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 3)
    {
        this->step = 4;

        if(this->additionalGoldGain > 0)
        {
            this->boardModel->printMessage("Through SHIPPING, you additionally gain %1 gold.");
            this->boardModel->printMessage(" ");
            this->boardModel->gainGold(this->additionalGoldGain);
        }
    }

    this->boardModel->printMessage(QString("The TRADE with %1 has ended.").arg(this->what));
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CULTURE_OF_THIEVES) && this->step == 4)
    {
        this->step = 5;
        this->boardModel->printMessage("Advance (CULTURE OF THIEVES):");
        this->boardModel->printMessage(QString("Do you want to try and steal gold from %1?")
                                      .arg(this->what));

        this->boardModel->setDoneButton(false);
        this->stealingDecision.setWindowTitle("Advance (CULTURE OF THIEVES)");
        this->stealingDecision.setText("Do you want to try and steal gold from %1?");
        this->stealingDecision.addButton(QMessageBox::Yes);
        this->stealingDecision.addButton(QMessageBox::No);
        connect(&this->stealingDecision, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(stealingDecided(StandardButton)));
        this->stealingDecision.show();
    }
    else
    {
        this->step = -1;
    }

    if(this->step == -1)
    {
        this->boardModel->unsetActiveRegion();
        this->nextInstruction->setKeepInstruction(false);
        this->nextInstruction->initInstruction();
        return this->nextInstruction;
    }

    return this;
}

void TradeInstruction::stealingDecided(QAbstractButton *button)
{
    this->boardModel->setDoneButton(true);
    this->boardModel->setDoneText("Done");
    this->step = -1;

    QMessageBox::ButtonRole buttonRole =  this->stealingDecision.buttonRole(button);

    switch(buttonRole)
    {
        case QMessageBox::YesRole:
            this->boardModel->printMessage(QString("Begin stealing from %1...")
                                          .arg(this->what));
            this->nextInstruction = new StealingInstruction(this->boardModel, this->what, this->nextInstruction);
            break;
        default:
            this->boardModel->printMessage("You decided not to steal any gold.");
            break;
    }

    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to Continue...");
    this->boardModel->printMessage(" ");
}
