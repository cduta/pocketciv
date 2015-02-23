#include "ExpeditionInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

ExpeditionInstruction::ExpeditionInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), gainGold(0), expeditionCardDrawn(false)
{
    this->nextInstruction->setKeepInstruction(true);
}

void ExpeditionInstruction::initInstruction()
{
    this->boardModel->sendMessage("EXPEDITION:");
    this->boardModel->sendMessage("Choose a region bordering on the frontier.");
    this->boardModel->sendMessage("Decimate any amount of tribes, then draw a card.");
    this->boardModel->sendMessage("Gain Gold equal to the decimated tribes minus the BLUE HEX number.");
    this->boardModel->sendMessage("At least 1 tribe has to remain in the region.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();
    return;
}

Instruction *ExpeditionInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(regionModel->getSelectedTribes() < regionModel->getTribes() - 1)
        {
            if(this->boardModel->refActiveRegion() == NULL &&
               this->boardModel->bordersOnFrontier(regionModel->getRegion()))
            {
                this->boardModel->setActiveRegion(regionModel->getRegion(), false);
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
    else if(button == Qt::RightButton)
    {
        if(this->boardModel->refActiveRegion() == regionModel)
        {
            if(regionModel->getSelectedTribes() > 0)
            {
                regionModel->setSelectedTribes(regionModel->getSelectedTribes() - 1);
                if(regionModel->getSelectedTribes() == 0)
                {
                    this->boardModel->unsetActiveRegion();
                    this->boardModel->sendMessage("Active Region unselected.");
                }
                else
                {
                    this->boardModel->sendMessage(QString("Selected %1 out of %2 tribes in region %2 for the expedition.")
                                                  .arg(regionModel->getSelectedTribes())
                                                  .arg(regionModel->getTribes())
                                                  .arg(regionModel->getRegion()));
                }
            }
        }
    }

    return this;
}

Instruction *ExpeditionInstruction::triggerDone()
{
    if(this->boardModel->refActiveRegion())
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

            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage(QString("You gain gold equal to the sent Tribes minus %1 (At least 0).").arg(blueHexNumber));
            this->boardModel->sendMessage(" ");

            this->boardModel->unsetActiveRegion();
            POCKET_CIV_END_OF_ERA_CHECK
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
        this->expeditionCardDrawn = false;
        this->boardModel->unselectAllSelectedTribes();
        this->boardModel->unsetActiveRegion();
    }
    else
    {
        this->gainGold = 0;
        this->expeditionCardDrawn = false;
        this->boardModel->unselectAllSelectedTribes();
        this->boardModel->unsetActiveRegion();
        this->boardModel->disableButtons();
        this->boardModel->enableMainPhaseButtons();
    }

    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
