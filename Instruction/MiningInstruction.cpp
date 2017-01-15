#include "MiningInstruction.hpp"

#include <QMessageBox>

#include "Instruction/EndOfEraInstruction.hpp"

MiningInstruction::MiningInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : boardModel(boardModel), nextInstruction(nextInstruction), tribesCost(3), step(0), mineCart(0)
{
    this->nextInstruction->setKeepInstruction(true);
}

void MiningInstruction::initInstruction()
{
    this->boardModel->printMessage("MINING:");
    this->boardModel->printMessage(QString("Choose a region without a mountain or volcano and decimate %1 tribes to start mining.")
                                  .arg(this->tribesCost));
    this->boardModel->printMessage("When you start mining, draw a card. If it has gold nuggets,");
    this->boardModel->printMessage("add that amount of gold nuggets to your mine cart. Otherwise, your mine cart is lost.");
    this->boardModel->printMessage("Continue this process until either you stop and gain the amount of gold nuggets");
    this->boardModel->printMessage("in your mine cart in gold or until your mine cart is lost and you gain no gold.");
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::SURVEYING))
    {
        this->boardModel->printMessage(" ");
        this->boardModel->printMessage("Advance (SURVEYING):");
        this->boardModel->printMessage("If the mine cart is lost but had any gold in it, gain one gold instead of none.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("Remember: At least 1 tribe has to remain anywhere in the Empire, when decimating Tribes!");
    this->boardModel->printMessage(" ");

    this->boardModel->printMessage("When you are done, press Done.");
    this->boardModel->printMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->setDoneButton(true);
    return;
}

Instruction *MiningInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(this->step == 0 &&
           (regionModel->hasMountain() || regionModel->hasVolcano()) &&
            regionModel->getTribes() >= 3 &&
            this->boardModel->getTribeCount() > 3)
        {
            regionModel->setTribes(regionModel->getTribes() - 3);
            this->boardModel->setActiveRegion(regionModel->getRegion(), false);
            this->step = 1;
            this->boardModel->printMessage(QString("You chose to mine in region %1.").arg(regionModel->getRegion()));
            this->boardModel->printMessage("Press Done to start mining...");
            this->boardModel->printMessage(" ");
        }
    }

    return this;
}

Instruction *MiningInstruction::triggerDone()
{
    if(this->step == 1)
    {
        this->step++;
        int currentGoldNuggets = this->boardModel->drawCard(true)->getGoldNuggets();

        this->mineCart += currentGoldNuggets;

        int previousMineCart = this->mineCart;

        if(currentGoldNuggets == 0)
        {
            this->mineCart = 0;
        }

        if(this->mineCart > 0)
        {
            this->boardModel->printMessage(QString("The mine cart now holds %1 gold.").arg(this->mineCart));
        }
        else
        {
            this->boardModel->printMessage("The mine cart and all its content was lost.");
            this->boardModel->printMessage(" ");

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::SURVEYING) && previousMineCart > 0)
            {
                this->boardModel->printMessage(QString("Even though the mine cart was lost with %1 gold,").arg(previousMineCart));
                this->boardModel->printMessage("you still receive one gold.");
                this->boardModel->printMessage(" ");
                this->boardModel->gainGold(1);
            }
        }

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 2)
    {
        this->step = 1;

        bool continueMining = false;

        if(this->mineCart > 0)
        {
            continueMining = QMessageBox::Yes ==
                    QMessageBox::StandardButton(
                        QMessageBox::question(NULL, "Mining...", QString("Gold in Mine Cart: %1\n\nIf you stop mining, you receive the amount of gold in the mine cart.\nDo you want to continue mining?").arg(this->mineCart),
                                              QMessageBox::Yes, QMessageBox::No));
        }

        if(!continueMining)
        {
            if(this->mineCart > 0)
            {
                this->boardModel->printMessage(" ");
                this->boardModel->printMessage(QString("You successfully receive %1 gold from mining.").arg(this->mineCart));
                this->boardModel->printMessage(" ");
                this->boardModel->gainGold(this->mineCart);
            }

            this->step = 0;
            this->mineCart = 0;

            this->boardModel->printMessage("Choose the next region to mine in or press Done to continue...");
            this->boardModel->printMessage(" ");
        }

        return this;
    }

    this->boardModel->unsetActiveRegion();
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();

    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
