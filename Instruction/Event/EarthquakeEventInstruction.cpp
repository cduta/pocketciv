#include "EarthquakeEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

#include "Common.hpp"

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

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MEDICINE))
    {
        this->boardModel->printMessage("Advance (MEDICINE):");
        this->boardModel->printMessage("After the earthquake, every affected region has 1 tribe added to it.");
        this->boardModel->printMessage(" ");
    }

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

            this->affectedRegions.insert(regionModel->getRegion(), regionModel);

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

            this->affectedRegions.insert(regionModel->getRegion(), regionModel);

            if(regionModel->getTribes() == 0)
            {
                this->tribeLossRegionModels.remove(regionModel->getRegion());
            }

            if(this->populationLoss == 0)
            {
                this->boardModel->printMessage(" ");
                this->boardModel->printMessage("Press Done to continue.");
                this->boardModel->printMessage(" ");
                return this;
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
            this->boardModel->printMessage(QString("Region %1 has a fault line.").arg(activeRegion->getRegion()));

            this->step = 2;

            this->boardModel->printMessage(" ");
            int currentCityAV = activeRegion->getCityAV();
            int savedCityAC = 0;

            activeRegion->decreaseCityAV(3);

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING) && activeRegion->getCityAV() < 1 && currentCityAV > 0)
            {
                savedCityAC = 1 - (currentCityAV - 3);
                activeRegion->setCityAV(1);
            }

            bool isAffected = currentCityAV != activeRegion->getCityAV() ||
                              activeRegion->hasTribes() || activeRegion->hasWonders();
            activeRegion->decimateTribes(4);
            activeRegion->decimateAllWonders();

            if(!isAffected)
            {
                this->boardModel->printMessage(QString("Region %1 is not affected by the earthquake..").arg(activeRegion->getRegion()));
                this->boardModel->printMessage(" ");
            }
            else
            {
                this->boardModel->printMessage(QString("The earthquake reduces %1 City AV, decimated 4 Tribes in region %2.").arg(3 - savedCityAC));
                this->boardModel->printMessage(QString("and all wonders in region %1.").arg(activeRegion->getRegion()));
                this->boardModel->printMessage(" ");

                if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING))
                {
                    this->boardModel->printMessage(" ");
                    this->boardModel->printMessage("Advance (ENGINEERING):");
                    this->boardModel->printMessage("Cities hit by the earthquake cannot have their City AV");
                    this->boardModel->printMessage("reduced below 1.");
                    this->boardModel->printMessage(" ");
                }

                this->affectedRegions.insert(activeRegion->getRegion(), activeRegion);
            }

            this->borderingRegions = this->boardModel->getAdjacentRegions(activeRegion->getRegion());

            foreach(int region, this->borderingRegions.keys())
            {
                RegionModel *regionModel = this->borderingRegions.value(region);
                if(!regionModel->hasFaultLine())
                {
                    this->faultLinelessRegionModels.insert(region, regionModel);
                }
            }

            this->boardModel->printMessage("The earthquake creates a fault line in two regions without a fault line");
            this->boardModel->printMessage(QString("bordering on region %1.").arg(activeRegion->getRegion()));
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
                    this->affectedRegions.unite(this->faultLinelessRegionModels);

                    this->boardModel->printMessage("Since there are only two bordering regions without fault lines.");
                    this->boardModel->printMessage("Both receive a fault line.");
                    this->boardModel->printMessage(" ");
                }
                else if(this->faultLinelessRegionModels.size() == 1)
                {
                    this->faultLinelessRegionModels.values().at(0)->setFaultLine(true);
                    this->affectedRegions.unite(this->faultLinelessRegionModels);

                    this->boardModel->printMessage("Since there is only one bordering regions without a fault line.");
                    this->boardModel->printMessage("Only this region receives a fault line.");
                    this->boardModel->printMessage(" ");
                }
                else
                {
                    this->boardModel->printMessage("But there is no region without a fault line bordering on the active region.");
                    this->boardModel->printMessage("Therefore: no faultlines are created.");
                    this->boardModel->printMessage(" ");
                }

                this->step = 3;
                this->faultLinesLeft = 0;
            }
        }
        else
        {
            this->boardModel->printMessage(QString("Region %1 has no fault line.").arg(activeRegion->getRegion()));
            this->boardModel->printMessage(" ");

            int savedCityAV = 0;
            activeRegion->decreaseCityAV(1);

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING) && activeRegion->getCityAV() < 1)
            {
                savedCityAV = 1;
                activeRegion->setCityAV(1);
            }

            activeRegion->decimateTribes(1);
            activeRegion->setFaultLine(true);

            this->affectedRegions.insert(activeRegion->getRegion(), activeRegion);

            this->boardModel->printMessage(QString("The earthquake decreased %1 City AV and decimated 1 Tribe in the active region.").arg(1 - savedCityAV));
            this->boardModel->printMessage("It also created a fault line in the active region.");
            this->boardModel->printMessage(" ");

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING))
            {
                this->boardModel->printMessage("Advance (ENGINEERING):");
                this->boardModel->printMessage("Cities hit by the EARTHQUAKE cannot have their City AV");
                this->boardModel->printMessage("reduced below 1.");
                this->boardModel->printMessage(" ");
            }

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

            if(regionModel->hasTribes())
            {
                this->tribeLossRegionModels.insert(region, regionModel);
            }
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

            this->affectedRegions.unite(this->tribeLossRegionModels);

            this->boardModel->printMessage("The amount of tribes in the affected regions is equal or less than the population loss.");
            this->boardModel->printMessage("Therefore, all tribes in those regions are decimated.");
        }
    }

    if(this->step == -1 && this->populationLoss == 0)
    {
        this->boardModel->unsetActiveRegion();

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::MEDICINE))
        {
            this->boardModel->printMessage("Advance (MEDICINE):");
            this->boardModel->printMessage(QString("One tribe is added to every affected region (%1).")
                                           .arg(Common::listUpRegions(this->affectedRegions.values())));
            this->boardModel->printMessage(" ");

            foreach(RegionModel *regionModel, this->affectedRegions.values())
            {
                regionModel->setTribes(regionModel->getTribes() + 1);
            }
        }

        return this->endEvent();
    }

    return this;
}
