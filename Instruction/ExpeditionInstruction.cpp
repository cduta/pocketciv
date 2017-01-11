#include "ExpeditionInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

#include <QtCore/qmath.h>
#include <QMessageBox>

ExpeditionInstruction::ExpeditionInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), gainGold(0), expeditionCardDrawn(false)
{
    this->nextInstruction->setKeepInstruction(true);
}

void ExpeditionInstruction::initInstruction()
{
    this->boardModel->printMessage("EXPEDITION:");
    this->boardModel->printMessage("Choose a region bordering on a frontier.");
    this->boardModel->printMessage("Decimate any amount of tribes, then draw a card.");
    this->boardModel->printMessage("Gain Gold equal to the decimated tribes minus the BLUE HEX number.");
    this->boardModel->printMessage("Remember: At least 1 tribe has to remain anywhere in the Empire,");
    this->boardModel->printMessage("when decimating Tribes!");
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::NAVIGATION))
    {
        this->boardModel->printMessage("Advance (NAVIGATION):");
        this->boardModel->printMessage("You may also choose a region bordering on a sea.");
        this->boardModel->printMessage("The SEA Expedition works like a normal Expedition, but");
        this->boardModel->printMessage("instead of the BLUE HEX number, use the GREEN SQUARE number.");
        this->boardModel->printMessage("If the region borders on a frontier as well, you can decide");
        this->boardModel->printMessage("what type of expediton this is going to be.");
        this->boardModel->printMessage(" ");
    }


    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MAGNETICS))
    {
        this->boardModel->printMessage("Advance (MAGNETICS):"); // Apply also to SEA Expedition.
        this->boardModel->printMessage("Before applying the BLUE HEX number, divide it by 2 (round up).");

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::NAVIGATION))
        {
            this->boardModel->printMessage("If you choose to do a SEA Expedition,");
            this->boardModel->printMessage("apply this to the GREEN SQUARE number.");
        }
        this->boardModel->printMessage(" ");
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CAVALRY))
    {
        this->boardModel->printMessage("Advance (CAVALRY):"); // ONLY for frontier expedtions.
        this->boardModel->printMessage("Each tribe sent into the frontier, counts as two tribes.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("When you are done, press Done.");
    this->boardModel->printMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->setDoneButton(true);
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
        if(regionModel->getTribes() > regionModel->getSelectedTribes() &&
           this->boardModel->getTribeCount() > regionModel->getSelectedTribes()+1)
        {
            if(this->boardModel->refActiveRegion() == NULL &&
               this->boardModel->bordersOnFrontier(regionModel->getRegion()))
            {
                this->boardModel->setActiveRegion(regionModel->getRegion(), false);
            }

            if(this->boardModel->refActiveRegion() == regionModel)
            {
                regionModel->setSelectedTribes(regionModel->getSelectedTribes() + 1);
                this->boardModel->printMessage(QString("Selected %1 out of %2 tribes in region %2 for the expedition.")
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
                regionModel->setSelectedTribes(regionModel->getSelectedTribes()-1);
                if(regionModel->getSelectedTribes() == 0)
                {
                    this->boardModel->unsetActiveRegion();
                    this->boardModel->printMessage("Active Region unselected.");
                }
                else
                {
                    this->boardModel->printMessage(QString("Selected %1 out of %2 tribes in region %3 for the expedition.")
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
            bool seaExpedition = false;

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::NAVIGATION) &&
               this->boardModel->bordersOnSea(this->boardModel->refActiveRegion()->getRegion()))
            {
                if(this->boardModel->bordersOnFrontier(this->boardModel->refActiveRegion()->getRegion()))
                {
                    int result = QMessageBox::question(NULL, "Sea or Frontier Expedition?", "Where do you want to send the Expedition?", "Frontier","Sea","Cancel");

                    if(result == 0)
                    {
                        seaExpedition = false;
                    }
                    else if(result == 1)
                    {
                        seaExpedition = true;
                    }
                    else if (result == 2)
                    {
                        this->boardModel->printMessage(QString("Canceled sending an Expedition from Region %1.").arg(this->boardModel->refActiveRegion()->getRegion()));
                        this->boardModel->printMessage(" ");
                        this->boardModel->unselectAllSelectedTribes();
                        this->boardModel->unsetActiveRegion();
                        return this;
                    }
                }
                else
                {
                    seaExpedition = true;
                }
            }

            this->expeditionCardDrawn = true;
            this->boardModel->decimateAllSelectedTribes();
            const EventCard *card = this->boardModel->drawCard();
            int expeditionCost;

            if(seaExpedition)
            {
                this->boardModel->printMessage(QString("Sending %1 tribes on an expedition into the sea...").arg(selectedTribes));
                this->boardModel->printMessage(" ");
                expeditionCost = card->getShapeNumbers().value(Event::GREEN_SQUARE, 0);
            }
            else
            {
                this->boardModel->printMessage(QString("Sending %1 tribes on an expedition into the frontier...").arg(selectedTribes));
                this->boardModel->printMessage(" ");
                expeditionCost = card->getShapeNumbers().value(Event::BLUE_HEXAGON, 0);
            }

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::MAGNETICS))
            {
                expeditionCost = qCeil(((double)expeditionCost)/2.0);
            }

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::CAVALRY))
            {
                this->gainGold = (selectedTribes*2) - expeditionCost;
            }
            else
            {
                this->gainGold = selectedTribes - expeditionCost;
            }

            if(this->gainGold < 0)
            {
                this->gainGold = 0;
            }

            this->boardModel->printMessage(QString("You gain gold equal to the sent Tribes minus %1.").arg(expeditionCost));
            this->boardModel->printMessage(" ");

            this->boardModel->unsetActiveRegion();
            POCKET_CIV_END_OF_ERA_CHECK
        }

        if(this->gainGold == 0)
        {
            this->boardModel->printMessage(QString("The expedition was a failure and you gain no gold."));
        }
        else
        {
            this->boardModel->printMessage(QString("The expedition was a success and you gain %1 gold.").arg(this->gainGold));
        }

        this->boardModel->printMessage(" ");
        this->boardModel->gainGold(this->gainGold);
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
