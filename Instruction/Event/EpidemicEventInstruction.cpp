#include "EpidemicEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

#include <QtCore/qmath.h>

EpidemicEventInstruction::EpidemicEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{
    this->nextInstruction->setKeepInstruction(false);
}

void EpidemicEventInstruction::initInstruction()
{
    this->boardModel->printMessage("EPIDEMIC:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(QString(" "));
    return;
}

Instruction *EpidemicEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != -1)
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
            this->boardModel->printMessage(QString("The EPIDEMIC moved to region %1.").arg(this->boardModel->refActiveRegion()->getRegion()));
            this->boardModel->printMessage(QString("This is the new active region."));
            this->boardModel->printMessage(QString(" "));
            return this->continueEpidemic();
        }
        else
        {
            this->boardModel->printMessage("No region selected.");
            this->boardModel->printMessage(QString(" "));
        }
    }

    return this;
}

Instruction *EpidemicEventInstruction::setActiveRegion()
{
    this->step = 1;
    this->drawActiveRegion();

    POCKET_CIV_END_OF_ERA_CHECK

    return NULL;
}

Instruction *EpidemicEventInstruction::checkActiveRegion()
{
    this->step = 2;
    if(this->boardModel->refActiveRegion()->getTribes() == 0)
    {
        this->boardModel->printMessage("But the active region has no tribes.");
        this->boardModel->printMessage("The EPIDEMIC will not spread.");
        this->boardModel->printMessage(QString(" "));
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }

    return NULL;
}

Instruction *EpidemicEventInstruction::initializePopulationLoss()
{
    this->step = 3;
    this->populationLoss = this->boardModel->drawCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
    this->boardModel->printMessage(QString("The population loss of this EPIDEMIC is %1.").arg(this->populationLoss));
    this->boardModel->printMessage(QString(" "));

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MEDICINE))
    {
        this->populationLoss = qFloor(((double)this->populationLoss)/2.0);

        this->boardModel->printMessage(QString("Advance (MEDICINE):"));
        this->boardModel->printMessage(QString("Divide the population loss by 2 (round down)."));
        this->boardModel->printMessage(QString(" "));
        this->boardModel->printMessage(QString("The population loss is now %1.").arg(this->populationLoss));
        this->boardModel->printMessage(QString(" "));
    }

    if(this->populationLoss <= 0)
    {
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }

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

    this->boardModel->printMessage(QString("The amount of tribes decimated in region %1 is %2.").arg(this->boardModel->refActiveRegion()->getRegion()).arg(this->oldTribes - newTribes));
    this->boardModel->printMessage(" ");

    this->populationLoss = this->populationLoss - this->oldTribes;

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::ROADBUILDING))
    {
        this->boardModel->printMessage("Advance (ROADBUILDING):");
        this->boardModel->printMessage("If this region has a city and no more tribes left now,");
        this->boardModel->printMessage("decrease the City AV by 2 and decimate the City if it has 0.");
        this->boardModel->printMessage(" ");

        if(this->oldTribes > 0 &&
           this->boardModel->refActiveRegion()->getTribes() == 0 &&
           this->boardModel->refActiveRegion()->hasCity())
        {
            this->boardModel->refActiveRegion()->decreaseCityAV(2);
            this->boardModel->refActiveRegion()->decimateZeroAVCity();
        }
    }

    if(this->populationLoss <= 0)
    {
        this->boardModel->printMessage(QString("The Population loss is now 0."));
        this->boardModel->printMessage(QString(" "));
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }

    this->boardModel->printMessage(QString("Population Loss left: %1").arg(this->populationLoss));
    this->boardModel->printMessage(QString(" "));

    if(overallTribes <= 2)
    {
        this->boardModel->printMessage(QString("The EPIDEMIC cannot spread further with 2 or less tribes left in the EMPIRE."));
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }

    QMap<int, RegionModel *> adjacentRegions;

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN) || this->boardModel->hasAdvanceAquired(AdvanceModel::ROADBUILDING))
    {
        QString advanceType = "";

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN) && this->boardModel->hasAdvanceAquired(AdvanceModel::ROADBUILDING))
        {
            advanceType = "(EQUESTRIAN)/(ROADBUILDING)";
        }
        else if(this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN))
        {
            advanceType = "(EQUESTRIAN)";
        }
        else if(this->boardModel->hasAdvanceAquired(AdvanceModel::ROADBUILDING))
        {
            advanceType = "(ROADBUILDING)";
        }

        this->boardModel->printMessage(QString("Advance %1:").arg(advanceType));
        this->boardModel->printMessage("The EPIDEMIC is able to spread into any");
        this->boardModel->printMessage("region not seperated by a SEA or FRONTIER.");
        this->boardModel->printMessage(" ");
        adjacentRegions = this->boardModel->getContinentRegions(this->boardModel->refActiveRegion()->getRegion());
    }
    else
    {
        adjacentRegions = this->boardModel->getAdjacentRegions(this->boardModel->refActiveRegion()->getRegion());
    }

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
        QString adjacentString = " adjacent ";
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN) || this->boardModel->hasAdvanceAquired(AdvanceModel::ROADBUILDING))
        {
            adjacentString = " ";
        }

        this->boardModel->printMessage(QString("No%1regions have any tribes and the EPIDEMIC is stopped.").arg(adjacentString));
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }
    else
    {
        QString adjacentString = " an adjacent ";
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN)  || this->boardModel->hasAdvanceAquired(AdvanceModel::ROADBUILDING))
        {
            adjacentString = " a ";
        }

        this->boardModel->printMessage(QString("Choose%1region with tribes into which the EPIDEMIC will spread."));
        this->boardModel->printMessage("When you chose a region, press Done...");
        this->boardModel->printMessage(" ");
        return this;
    }
}
