#include "MainPhaseInstruction.hpp"

#include "UpkeepInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

MainPhaseInstruction::MainPhaseInstruction(BoardModel *boardModel)
    : Instruction(),
      boardModel(boardModel),
      initialized(false),
      buildCity(false),
      expedition(false),
      expeditionCardDrawn(false),
      gainGold(0)
{}

void MainPhaseInstruction::initInstruction()
{
    this->boardModel->sendMessage("Main Phase:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done, to begin.");
    return;
}

Instruction *MainPhaseInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(this->buildCity)
        {
            if(!regionModel->hasCity() && regionModel->getTribes() >= 4+1)
            {
                regionModel->setTribes(regionModel->getTribes() - 4);
                regionModel->setCityAV(1);
                regionModel->setCity(true);
            }
        }
        if(this->buildFarm)
        {
            if(!regionModel->hasFarm() && regionModel->getTribes() >= 2+1 && regionModel->hasForest())
            {
                regionModel->setTribes(regionModel->getTribes() - 2);
                regionModel->setForest(false);
                regionModel->setFarm(true);
            }
        }
        if(this->expedition)
        {
            if(regionModel->getSelectedTribes() < regionModel->getTribes() - 1)
            {
                if(this->boardModel->refActiveRegion() == NULL &&
                   this->boardModel->bordersOnFrontier(regionModel->getRegion()))
                {
                    this->boardModel->setActiveRegion(regionModel->getRegion());
                }

                if(this->boardModel->refActiveRegion() == regionModel)
                {
                    regionModel->setSelectedTribes(regionModel->getSelectedTribes() + 1);
                    this->boardModel->sendMessage(QString("Selected %1 out of %2 tribes in region %2 for the expedition.")
                                                  .arg(regionModel->getSelectedTribes())
                                                  .arg(regionModel->getTribes())
                                                  .arg(regionModel->getRegion()));
                }
            }
        }
    }
    else if(button == Qt::RightButton)
    {
        if(this->expedition &&
           this->boardModel->refActiveRegion() == regionModel &&
           regionModel->getSelectedTribes() > 0)
        {
            regionModel->setSelectedTribes(regionModel->getSelectedTribes() - 1);
            this->boardModel->sendMessage(QString("Selected %1 out of %2 tribes in region %2 for the expedition.")
                                          .arg(regionModel->getSelectedTribes())
                                          .arg(regionModel->getTribes())
                                          .arg(regionModel->getRegion()));
        }
    }

    return this;
}

Instruction *MainPhaseInstruction::triggerDone()
{
    if(this->expedition)
    {
        int selectedTribes = this->boardModel->refActiveRegion()->getSelectedTribes();

        if(!this->expeditionCardDrawn && selectedTribes > 0)
        {
            this->expeditionCardDrawn = true;
            this->boardModel->sendMessage(QString("Sending %1 tribes on an expedition into the frontier...").arg(selectedTribes));
            this->boardModel->sendMessage(" ");
            this->boardModel->decimateAllSelectedTribes();
            const EventCard * card = this->boardModel->drawCard();
            int blueHexNumber = card->getShapeNumbers().value(Event::BLUE_HEXAGON, 0);
            this->gainGold = selectedTribes - blueHexNumber;

            if(this->gainGold < 0)
            {
                this->gainGold = 0;
            }

            this->boardModel->unsetActiveRegion();
            POKET_CIV_END_OF_ERA_CHECK
        }

        if(this->gainGold == 0)
        {
            this->boardModel->sendMessage(QString("The expedition was a failure and you gain no gold."));
        }
        else
        {
            this->boardModel->sendMessage(QString("The expedition was a success and you gain %1 gold.").arg(this->gainGold));
        }

        this->boardModel->sendMessage(" ");
        this->boardModel->setGold(this->boardModel->getGold() + this->gainGold);
        this->boardModel->disableButtons();
        this->boardModel->enableMainPhaseButtons();
        this->boardModel->decimateAllSelectedTribes();
        this->gainGold = 0;
        this->expedition = false;
        this->expeditionCardDrawn = false;
        this->boardModel->unselectAllSelectedTribes();
        this->boardModel->unsetActiveRegion();
        return this;
    }

    if(this->buildCity || this->buildFarm)
    {
        this->boardModel->disableButtons();
        this->boardModel->enableMainPhaseButtons();
        this->buildCity = false;
        this->buildFarm = false;
        return this;
    }

    if(!this->initialized)
    {
        this->boardModel->clearMessages();
        this->boardModel->sendMessage("Main Phase:");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("You can do any amount of actions by clicking on any of the buttons.");
        this->boardModel->sendMessage("After applying any actions to a region, at least 1 tribe has to remain.");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("When you are Done, press Done.");
        this->boardModel->sendMessage(" ");
        this->boardModel->enableMainPhaseButtons();
        this->initialized = true;
        return this;
    }

    if(this->initialized)
    {
        this->boardModel->disableButtons();
        this->boardModel->enableDoneButton();
        Instruction *next = new UpkeepInstruction(this->boardModel);
        next->initInstruction();
        return next;
    }

    return this;
}

Instruction *MainPhaseInstruction::triggerBuildCity()
{
    this->buildCity = true;
    this->boardModel->sendMessage("Build City:");
    this->boardModel->sendMessage("Choose a region without a city.");
    this->boardModel->sendMessage("Decimate 4 tribes on it to build a city with 1 AV.");
    this->boardModel->sendMessage("At least 1 tribe has to remain in the region.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();

    return this;
}

Instruction *MainPhaseInstruction::triggerBuildFarm()
{
    this->buildFarm = true;
    this->boardModel->sendMessage("Build Farm:");
    this->boardModel->sendMessage("Choose a region without a farm and with a forest.");
    this->boardModel->sendMessage("Decimate 2 tribes and the forest to build a farm.");
    this->boardModel->sendMessage("At least 1 tribe has to remain in the region.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();

    return this;
}

Instruction *MainPhaseInstruction::triggerExpedition()
{
    this->expedition = true;
    this->boardModel->sendMessage("Expedition:");
    this->boardModel->sendMessage("Choose a region bordering on the frontier.");
    this->boardModel->sendMessage("Decimate any amount of tribes, then draw a card.");
    this->boardModel->sendMessage("Gain Gold equal to the decimated tribes minus the BLUE HEX number.");
    this->boardModel->sendMessage("At least 1 tribe has to remain in the region.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();

    return this;
}

Instruction *MainPhaseInstruction::triggerAquireAdvance()
{
    // TODO: Aquire Advance

    return this;
}

Instruction *MainPhaseInstruction::triggerBuildWonder()
{
    // TODO: Aquire Wonder

    return this;
}
