#include "StealingInstruction.hpp"

#include <qmath.h>

#include "Instruction/EndOfEraInstruction.hpp"
#include "Instruction/AttackInstruction.hpp"

StealingInstruction::StealingInstruction(BoardModel *boardModel, const QString &what, Instruction *nextInstruction)
    : boardModel(boardModel), what(what), nextInstruction(nextInstruction), step(0), stash(0), done(false)
{
    this->nextInstruction->setKeepInstruction(true);
}

void StealingInstruction::initInstruction()
{
    this->boardModel->sendMessage(QString("STEALING FROM %1")
                                  .arg(this->what));
    this->boardModel->sendMessage(QString("Try stealing from %1.")
                                  .arg(this->what));
    this->boardModel->sendMessage("As long as you draw event cards with gold nuggets on them, you gain gold and may keep on stealing.");
    this->boardModel->sendMessage(QString("But once you've stolen gold, but do not draw gold nuggets when trying to steal more, you are caught, "
                                          "the %1 will attack and you gain only half the gold you would otherwise.")
                                  .arg(this->what));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to Continue...");
    this->boardModel->sendMessage(" ");
    return;
}

Instruction *StealingInstruction::triggerDone()
{
    if(this->done)
    {
        this->nextInstruction->setKeepInstruction(false);
        this->nextInstruction->initInstruction();
        return this->nextInstruction;
    }

    if(this->step == 0)
    {
        this->step = 1;

        int goldNuggets = this->boardModel->drawCard()->getGoldNuggets();
        this->stash += goldNuggets;

        if(goldNuggets > 0)
        {
            this->boardModel->sendMessage(QString("You stole %1 gold nuggets on your first try.")
                                          .arg(goldNuggets));
        }
        else
        {
            this->boardModel->sendMessage("You draw no gold nuggets.");
            this->boardModel->sendMessage("The stealing attempt failed and you've NOT been caught.");
            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage("Press Done to Continue...");
            this->boardModel->sendMessage(" ");
            this->done = true;
            this->step = -1;
        }

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->decision.setWindowTitle(QString("Stealing from %1?").arg(this->what));
        this->decision.setIcon(QMessageBox::Question);
        this->decision.setText(QString("If you stop stealing now, you gain %1 gold.\nDo you want to continue stealing gold from %2?")
                               .arg(this->stash)
                               .arg(this->what));
        this->decision.addButton(QMessageBox::Yes);
        this->decision.addButton(QMessageBox::No);

        connect(&this->decision, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(decisionMade(QAbstractButton*)));

        this->boardModel->setDoneButton(false);
        this->decision.show();
    }

    if(this->step == 2)
    {
        int goldNuggets = this->boardModel->drawCard()->getGoldNuggets();
        this->stash += goldNuggets;

        if(goldNuggets > 0)
        {
            this->boardModel->sendMessage(QString("You steal %1 gold nuggets for a total of %2 gold nuggets stolen so far.")
                                          .arg(goldNuggets).arg(this->stash));
            this->step = 1;
        }
        else
        {
            this->stash = qCeil(((double)this->stash)/2.0);

            this->boardModel->sendMessage(QString("Your attempt failed and you've been caught.\nYou fled with half your stash (rounded up) for a total of %1 gold gained.")
                                          .arg(this->stash));
            this->boardModel->sendMessage(QString("But, the %1 are attacking now.")
                                          .arg(this->what));
            this->step = 3;
        }

        this->boardModel->sendMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 3)
    {
        this->step = 4;

        this->boardModel->setActiveRegion(this->boardModel->drawCard()->getShapeNumbers().value(Event::RED_CIRCLE, 0), true);
        this->boardModel->sendMessage(QString("The active region in which the %1 are attacking is %2.")
                                      .arg(this->what)
                                      .arg(this->boardModel->refActiveRegion()->getRegion()));
        this->boardModel->sendMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 4)
    {
        this->step = 5;
        int attackForce = this->boardModel->drawCard()->getShapeNumbers().value(Event::GREEN_SQUARE, 0) +
                          this->boardModel->drawCard()->getShapeNumbers().value(Event::BLUE_HEXAGON, 0);
        this->nextInstruction = new AttackInstruction(this->boardModel, this->nextInstruction, this->what, attackForce);

        this->boardModel->sendMessage(QString("Adding up the GREEN SQUARE and BLUE HEXAGON number results in an attacking force of %1.").arg(attackForce));
        this->boardModel->sendMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 5)
    {
        this->nextInstruction->initInstruction();
        return this->nextInstruction;
    }

    return this;
}

void StealingInstruction::decisionMade(QAbstractButton *button)
{
    this->boardModel->setDoneButton(true);

    QMessageBox::StandardButton buttonRole = QMessageBox::StandardButton(this->decision.standardButton(button));

    switch(buttonRole)
    {
        case QMessageBox::Yes:
            this->step = 2;
            this->boardModel->sendMessage("You try to steal some more gold.\nPress Done to continue.");
            break;
        default:
            this->done = true;
            this->step = -1;
            this->boardModel->gainGold(this->stash);
            this->boardModel->sendMessage(QString("You decided to stop stealing and collect the gold you stole so far.\n"
                                          "You receive %1 Gold.\n"
                                          "Press Done to Continue...")
                                          .arg(this->stash));
            break;
    }

    this->boardModel->sendMessage(" ");

    return;
}
