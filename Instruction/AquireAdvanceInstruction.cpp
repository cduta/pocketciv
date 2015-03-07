#include "AquireAdvanceInstruction.hpp"

AquireAdvanceInstruction::AquireAdvanceInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : boardModel(boardModel),
      nextInstruction(nextInstruction),
      advancesDialog(NULL),
      aquireAdvances(false)
{
    this->nextInstruction->setKeepInstruction(true);
}

void AquireAdvanceInstruction::initInstruction()
{
    this->boardModel->sendMessage("AQUIRE ADVANCE:");

    if(this->boardModel->getCityAVCount() > this->boardModel->getAdvancesAquired().size())
    {
        this->boardModel->sendMessage("Choose a region with a city to aquire an advance");
        this->boardModel->sendMessage("using the ressources of that region.");
        this->boardModel->sendMessage("You can aquire one advance per region per turn.");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("When you are done, press Done.");
        this->boardModel->sendMessage(" ");
        this->boardModel->disableButtons();
        this->boardModel->setDoneButton(true);
        this->boardModel->selectAdvanceableRegions();
    }
    else
    {
        this->boardModel->sendMessage("It's not possible for you to aquire an advance,");
        this->boardModel->sendMessage("while your the amount of advances you aquired is equal or higher than");
        this->boardModel->sendMessage("the total amount of City AV in your empire.");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage(QString("Aquired advances: %1")
                                      .arg(this->boardModel->getAdvancesAquired().size()));
        this->boardModel->sendMessage(QString("Total City AV: %1")
                                      .arg(this->boardModel->getCityAVCount()));
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Press Done to continue.");
        this->boardModel->sendMessage(" ");

        this->boardModel->disableButtons();
        this->boardModel->setDoneButton(true);
    }
    return;
}

Instruction *AquireAdvanceInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel)
    {
        if(button == Qt::LeftButton && regionModel->isSelected() && this->boardModel->getCityAVCount() > this->boardModel->getAdvancesAquired().size())
        {
            this->aquireAdvances = true;
            this->boardModel->setDoneButton(false);
            this->boardModel->setActiveRegion(regionModel->getRegion(), false);
            this->advancesDialog = new AdvancesDialog(boardModel, AdvanceItem::AQUIRE);
            connect(this->advancesDialog, SIGNAL(finished(int)), this, SLOT(doneAquiringAdvances()));
            this->advancesDialog->show();
        }
    }

    return this;
}

Instruction *AquireAdvanceInstruction::triggerDone()
{
    if(this->aquireAdvances)
    {
        return this;
    }

    this->boardModel->unselectAllRegions();
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();
    this->nextInstruction->setKeepInstruction(false);
    return this->nextInstruction;
}

void AquireAdvanceInstruction::doneAquiringAdvances()
{
    disconnect(this->advancesDialog, SIGNAL(finished(int)), this, SLOT(doneAquiringAdvances()));
    this->advancesDialog->disconnetAll();

    this->boardModel->setDoneButton(true);
    this->aquireAdvances = false;
    this->boardModel->unsetActiveRegion();
    this->boardModel->unselectAllRegions();
    this->boardModel->selectAdvanceableRegions();

    this->advancesDialog->deleteLater();
    return;
}
