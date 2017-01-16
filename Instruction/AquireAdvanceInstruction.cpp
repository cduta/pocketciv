#include "AquireAdvanceInstruction.hpp"

AquireAdvanceInstruction::AquireAdvanceInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : boardModel(boardModel),
      nextInstruction(nextInstruction),
      advancesDialog(NULL)
{
    this->nextInstruction->setKeepInstruction(true);
}

void AquireAdvanceInstruction::initInstruction()
{
    this->boardModel->printMessage("AQUIRE ADVANCE:");

    if(this->boardModel->getCityAVCount() > this->boardModel->getAdvancesAquired().size())
    {
        this->boardModel->printMessage("Choose a region with a city to aquire an advance");
        this->boardModel->printMessage("using the ressources of that region.");
        this->boardModel->printMessage("You can aquire one advance per region per turn.");
        this->boardModel->printMessage("Remember: At least 1 tribe has to remain anywhere in the Empire, when decimating Tribes!");
        this->boardModel->printMessage(" ");

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::MACHINING))
        {
            this->boardModel->printMessage("Advance (MACHINING):");
            this->boardModel->printMessage("Wood (Forest) and stone (Mountain/Volcano) become interchangable.");
            this->boardModel->printMessage("A Mountain/Volcano may create wood instead of stone and a Forest");
            this->boardModel->printMessage("may create stone instead of wood.");
            this->boardModel->printMessage(" ");
        }

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::STORY_TELLING))
        {
            this->boardModel->printMessage("Advance (STORY TELLING):");
            this->boardModel->printMessage("Pay one less Tribe when Aquiring: MYTHOLOGY, MUSIC, LITERACY,");
            this->boardModel->printMessage("PHILOSOPHY, ARTS, THEATER, ORGANIZED RELIGION and/or LAW.");
            this->boardModel->printMessage(" ");
        }

        this->boardModel->printMessage("When you are done, press Done.");
        this->boardModel->printMessage(" ");
        this->boardModel->disableButtons();
        this->boardModel->setDoneButton(true);
        this->boardModel->selectAdvanceableRegions();
    }
    else
    {
        this->boardModel->printMessage("It's not possible for you to aquire an advance,");
        this->boardModel->printMessage("while your the amount of advances you aquired is equal or higher than");
        this->boardModel->printMessage("the total amount of City AV in your empire.");
        this->boardModel->printMessage(" ");
        this->boardModel->printMessage(QString("Aquired advances: %1")
                                      .arg(this->boardModel->getAdvancesAquired().size()));
        this->boardModel->printMessage(QString("Total City AV: %1")
                                      .arg(this->boardModel->getCityAVCount()));
        this->boardModel->printMessage(" ");
        this->boardModel->printMessage("Press Done to continue.");
        this->boardModel->printMessage(" ");

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
            this->boardModel->setAquiringAdvances(true);
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
    if(this->boardModel->isAquiringAdvances())
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

    this->boardModel->setAquiringAdvances(false);
    this->boardModel->setDoneButton(true);
    this->boardModel->unsetActiveRegion();
    this->boardModel->unselectAllRegions();
    this->boardModel->selectAdvanceableRegions();

    this->advancesDialog->deleteLater();
    return;
}
