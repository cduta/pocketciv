#include "EarthquakeEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

EarthquakeEventInstruction::EarthquakeEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event)
{
    this->step = 0;
    this->faultLinesLeft = 0;
    this->populationLoss = 0;
}

void EarthquakeEventInstruction::initInstruction()
{
    this->boardModel->printMessage("EARTHQUAKE:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *EarthquakeEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || (this->step != -1 && this->step != 2))
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(this->step == 2 && this->faultLinelessRegionModels.contains(regionModel->getRegion()) && this->faultLinesLeft > 0)
        {
            regionModel->setFaultLine(true);
            this->faultLinesLeft--;
            this->faultLinelessRegionModels.remove(regionModel->getRegion());

            this->boardModel->printMessage(QString("Region %1 received a fault line. Fault lines left: %2.")
                                          .arg(regionModel->getRegion())
                                          .arg(this->faultLinesLeft));

            if(this->faultLinesLeft == 0)
            {
                this->boardModel->printMessage(" ");
                this->boardModel->printMessage("Press Done to continue.");
                this->boardModel->printMessage(" ");
                this->step = 3;
            }
        }
        else if(this->step == -1 && this->tribeLossRegionModels.contains(regionModel->getRegion()) && this->populationLoss > 0)
        {
            regionModel->decimateTribes(1);
            this->populationLoss--;

            this->boardModel->printMessage(QString("Region %1 received population loss. Population loss left: %2.")
                                          .arg(regionModel->getRegion())
                                          .arg(this->populationLoss));

            if(regionModel->getTribes() == 0)
            {
                this->tribeLossRegionModels.remove(regionModel->getRegion());
            }

            if(this->populationLoss == 0)
            {
                return this->endEvent();
            }
        }
    }

    return this;
}

Instruction *EarthquakeEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;
        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        RegionModel *activeRegion = this->boardModel->refActiveRegion();

        if(activeRegion->hasFaultLine())
        {
            this->boardModel->printMessage("The active region has a fault line.");

            this->step = 2;
            activeRegion->decreaseCityAV(3);
            activeRegion->decimateTribes(4);
            activeRegion->decimateWonders();

            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("The earthquake decreased 3 City AV and decimated 4 Tribes in the active region.");
            this->boardModel->printMessage("It also decimated all wonders in the active region.");

            this->borderingRegions = this->boardModel->getAdjacentRegions(activeRegion->getRegion());

            foreach(int region, this->borderingRegions.keys())
            {
                RegionModel *regionModel = this->borderingRegions.value(region);
                if(!regionModel->hasFaultLine())
                {
                    this->faultLinelessRegionModels.insert(region, regionModel);
                }
            }

            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("The earthquake creates a fault line in two regions without a fault line");
            this->boardModel->printMessage("bordering on the active region.");
            this->boardModel->printMessage(" ");

            if(this->faultLinelessRegionModels.size() > 2)
            {
                this->faultLinesLeft = 2;

                this->boardModel->printMessage("You decide, which of these regions receives a fault line.");
                this->boardModel->printMessage(" ");
            }
            else
            {
                if(this->faultLinelessRegionModels.size() == 2)
                {
                    this->faultLinelessRegionModels.values().at(0)->setFaultLine(true);
                    this->faultLinelessRegionModels.values().at(1)->setFaultLine(true);

                    this->boardModel->printMessage("Since there are only two bordering regions without fault lines.");
                    this->boardModel->printMessage("Both receive a fault line.");
                    this->boardModel->printMessage(" ");
                }
                else if(this->faultLinelessRegionModels.size() == 1)
                {
                    this->faultLinelessRegionModels.values().at(0)->setFaultLine(true);

                    this->boardModel->printMessage("Since there is only one bordering regions without a fault line.");
                    this->boardModel->printMessage("Only this region receives a fault line.");
                    this->boardModel->printMessage(" ");
                }
                else
                {
                    this->boardModel->printMessage("But there is no region without a fault line bordering on the active region.");
                    this->boardModel->printMessage("Therefore, nothing happens.");
                    this->boardModel->printMessage(" ");
                }

                this->step = 3;
                this->faultLinesLeft = 0;
            }
        }
        else
        {
            this->boardModel->printMessage("The active region has no fault line.");

            activeRegion->decreaseCityAV(1);
            activeRegion->decimateTribes(1);
            activeRegion->setFaultLine(true);

            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("The earthquake decreased 1 City AV and decimated 1 Tribe in the active region.");
            this->boardModel->printMessage("It also created a fault line in the active region.");
            this->boardModel->printMessage(" ");

            this->step = 3;
        }

        if(this->step == 3)
        {
            this->boardModel->printMessage("Press Done to continue.");
            this->boardModel->printMessage(" ");
        }

        return this;
    }

    if(this->step == 3)
    {
        this->step = 4;

        this->populationLoss = this->boardModel->drawCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
        this->boardModel->printMessage("The earthquake affects the tribes in the active region and its bordering regions.");
        this->boardModel->printMessage("These affected regions suffer population loss.");
        this->boardModel->printMessage(QString("The population loss is %1.").arg(this->populationLoss));
        this->boardModel->printMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 4 && this->populationLoss > 0)
    {
        this->step = -1;

        int totalTribes = 0;

        foreach(int region, this->borderingRegions.keys())
        {
            RegionModel *regionModel = this->borderingRegions.value(region);

            totalTribes += regionModel->getTribes();
            this->tribeLossRegionModels.insert(region, regionModel);
        }

        if(totalTribes > this->populationLoss)
        {
            this->boardModel->printMessage("Distribute all the population loss in the affected regions with tribes.");
            this->boardModel->printMessage("Reduce a tribe for each population loss distributed in the affected regions.");
            this->boardModel->printMessage(" ");
        }
        else
        {
            this->populationLoss = 0;

            foreach(RegionModel *regionModel, this->tribeLossRegionModels.values())
            {
                regionModel->setTribes(0);
            }

            this->boardModel->printMessage("The amount of tribes in the affected regions is equal or less than the population loss.");
            this->boardModel->printMessage("Therefore, all tribes in those regions are decimated.");
        }
    }

    if(this->step == -1 && this->populationLoss == 0)
    {
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }

    return this;
}
