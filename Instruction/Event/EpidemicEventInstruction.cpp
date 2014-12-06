#include "EpidemicEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

EpidemicEventInstruction::EpidemicEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{
    this->nextInstruction->setKeepInstruction(false);
}

void EpidemicEventInstruction::initInstruction()
{
    this->boardModel->sendMessage("EPIDEMIC:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press done to continue.");
    return;
}

Instruction *EpidemicEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
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
        next = this->setActiveRegion();

        if(next != NULL)
        {
            return next;
        }
    }

    if(this->step == 1)
    {
        next = this->checkActiveRegion();

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

Instruction *EpidemicEventInstruction::setActiveRegion()
{
    this->step = 1;
    this->drawActiveRegion();

    POKET_CIV_END_OF_ERA_CHECK

    return NULL;
}

Instruction *EpidemicEventInstruction::checkActiveRegion()
{
    this->step = 2;
    if(this->boardModel->refActiveRegion()->getTribes() == 0)
    {
        this->boardModel->sendMessage("But the active region has no tribes.");
        this->boardModel->sendMessage("The EPIDEMIC will not spread.");
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
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
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }

    this->boardModel->sendMessage(QString("Population Loss left: %1").arg(this->populationLoss));

    if(overallTribes <= 2)
    {
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage(QString("The EPIDEMIC cannot spread further with 2 or less tribes left in the EMPIRE."));
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
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
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }
    else
    {
        this->boardModel->sendMessage("Choose the region with tribes into which the EPIDEMIC will spread.");
        this->boardModel->sendMessage("When you chose a region, press Done...");
        this->boardModel->sendMessage(" ");
        return this;
    }
}
