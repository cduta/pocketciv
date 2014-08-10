#include "EpidemicEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

EpidemicEventInstruction::EpidemicEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), step(0), event(event)
{
    this->nextInstruction->setKeepInstruction(false);
}

void EpidemicEventInstruction::initInstruction()
{
    this->originalCard = this->boardModel->refOriginalCard();
    this->boardModel->sendMessage(QString("Event: EPIDEMIC"));
    this->boardModel->sendMessage("Press done to continue.");
}

Instruction *EpidemicEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(this->boardModel->refActiveRegion() == regionModel)
        {
            return this;
        }

        QMap<int, RegionModel *> adjacentRegions = this->boardModel->getAdjacentRegions(this->boardModel->refActiveRegion()->getRegion());

        if(regionModel->getTribes() > 0 && adjacentRegions.values().contains(regionModel))
        {
            QMap<int, RegionModel *> selectedRegions = this->boardModel->getSelectedRegions();
            if(selectedRegions.size() == 1)
            {
                this->boardModel->setSelectRegion(selectedRegions.values().first()->getRegion(), false);
            }

            this->boardModel->setSelectRegion(regionModel->getRegion(), true);
        }
    }
    return this;
}

Instruction *EpidemicEventInstruction::triggerDone()
{
    Instruction *next;

    if(this->step == 0)
    {
        next = this->drawFirstCard();

        if(next != NULL)
        {
            return next;
        }
    }

    if(this->step == 1)
    {
        next = this->setActiveRegion();

        if(next != NULL)
        {
            return next;
        }
    }

    if(this->step == 2)
    {
        next = this->initializePopulationLoss();

        if(next != NULL)
        {
            return next;
        }
    }

    if(this->step == 3)
    {
        next = this->continueEpidemic();

        if(next != NULL)
        {
            return next;
        }
    }

    if(this->step == -1) // Move
    {
        QMap<int, RegionModel *> selectedRegions = this->boardModel->getSelectedRegions();
        if(selectedRegions.size() == 1)
        {
            this->boardModel->setActiveRegion(selectedRegions.values().first()->getRegion());
            this->boardModel->unselectAllRegions();
            this->boardModel->sendMessage(QString("The EPIDEMIC moved to region %1.").arg(this->boardModel->refActiveRegion()->getRegion()));
            this->boardModel->sendMessage(QString("This is the new active region."));
            return this->continueEpidemic();
        }
        else
        {
            this->boardModel->sendMessage("No region selected.");
        }
    }

    return this;
}

Instruction *EpidemicEventInstruction::drawFirstCard()
{
    this->step = 1;
    this->first = this->boardModel->drawCard();

    if(this->boardModel->isEndOfEra())
    {
        return new EndOfEraInstruction(this->boardModel, this);
    }

    return NULL;
}

Instruction *EpidemicEventInstruction::setActiveRegion()
{
    this->step = 2;
    this->boardModel->setActiveRegion(this->first->getShapeNumbers().value(Event::RED_CIRCLE));
    this->boardModel->sendMessage(QString("The active region is %1.").arg(this->boardModel->refActiveRegion()->getRegion()));

    if(this->boardModel->refActiveRegion()->getTribes() == 0)
    {
        this->boardModel->sendMessage("But the active region has no tribes.");
        this->boardModel->sendMessage("The EPIDEMIC will not spread.");
        this->boardModel->sendMessage("The event ends...");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Press Done to continue.");
        this->boardModel->sendMessage(" ");
        this->boardModel->unsetActiveRegion();
        this->nextInstruction->setKeepInstruction(false);
        this->nextInstruction->initInstruction();
        return this->nextInstruction;
    }

    return NULL;
}

Instruction *EpidemicEventInstruction::initializePopulationLoss()
{
    this->step = 3;
    this->populationLoss = this->boardModel->drawCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
    this->boardModel->sendMessage(QString("The population loss total of this EPIDEMIC is %1.").arg(this->populationLoss));

    return NULL;
}

Instruction *EpidemicEventInstruction::continueEpidemic()
{
    this->step = -1;
    this->oldTribes = this->boardModel->refActiveRegion()->getTribes();
    int newTribes = this->oldTribes - populationLoss;

    if(newTribes < 0)
    {
        newTribes = 0;
    }

    this->boardModel->refActiveRegion()->setTribes(newTribes);
    int overallTribes = this->boardModel->getTribeCount();

    this->boardModel->sendMessage(QString("The tribes decimated in the active region are %1.").arg(this->oldTribes - newTribes));
    this->populationLoss = this->populationLoss - this->oldTribes;

    if(this->populationLoss <= 0)
    {
        this->boardModel->sendMessage(QString("The Population loss is 0."));
        this->boardModel->sendMessage("The event ends...");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Press Done to continue.");
        this->boardModel->sendMessage(" ");
        this->boardModel->unsetActiveRegion();
        this->nextInstruction->setKeepInstruction(false);
        this->nextInstruction->initInstruction();
        return this->nextInstruction;
    }

    this->boardModel->sendMessage(QString("Population Loss left: %1").arg(this->populationLoss));

    if(overallTribes <= 2)
    {
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage(QString("The EPIDEMIC cannot spread with 2 or less tribes left in the empire."));
        this->boardModel->sendMessage("The event ends...");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Press Done to continue.");
        this->boardModel->sendMessage(" ");
        this->boardModel->unsetActiveRegion();
        this->nextInstruction->setKeepInstruction(false);
        this->nextInstruction->initInstruction();
        return this->nextInstruction;
    }

    QMap<int, RegionModel *> adjacentRegions = this->boardModel->getAdjacentRegions(this->boardModel->refActiveRegion()->getRegion());
    QMap<int, RegionModel *> adjacentRegionsWithTribes;

    foreach(int region, adjacentRegions.keys())
    {
        if(adjacentRegions[region]->getTribes() > 0)
        {
            adjacentRegionsWithTribes.insert(region, adjacentRegions[region]);
        }
    }

    if(adjacentRegionsWithTribes.size() == 0)
    {
        this->boardModel->sendMessage(QString("No adjacent regions has any tribes and the EPIDEMIC is stopped."));
        this->boardModel->sendMessage("The event ends...");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Press Done to continue.");
        this->boardModel->sendMessage(" ");
        this->boardModel->unsetActiveRegion();
        this->nextInstruction->setKeepInstruction(false);
        this->nextInstruction->initInstruction();
        return this->nextInstruction;
    }
    else
    {
        this->boardModel->sendMessage("Choose the region with tribes into which the EPIDEMIC will spread.");
        this->boardModel->sendMessage("When you chose a region, press Done...");
        this->boardModel->sendMessage(" ");
        return this;
    }
}
