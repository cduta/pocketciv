#include "VolcanoEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

VolcanoEventInstruction::VolcanoEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void VolcanoEventInstruction::initInstruction()
{
    this->boardModel->printMessage("VOLCANO:");
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MEDICINE))
    {
        this->boardModel->printMessage("Advance (MEDICINE):");
        this->boardModel->printMessage("After the volcano, every affected region has 1 tribe added to it.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *VolcanoEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step++;
        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        RegionModel *activeRegion = this->boardModel->refActiveRegion();

        if(activeRegion->hasMountain())
        {
            this->boardModel->printMessage(QString("The mountain in region %1 becomes an volcano and errupts.").arg(activeRegion->getRegion()));
            this->boardModel->printMessage(" ");

            activeRegion->setMountain(false);
            activeRegion->setVolcano(true);

            this->affectedRegions.insert(activeRegion->getRegion(), activeRegion);

            int currentCityAV = activeRegion->getCityAV();
            int decimateCityAV = 2;

            this->boardModel->printMessage(QString("The erruption reduces the City AV by %1,").arg(decimateCityAV));
            this->boardModel->printMessage("decimates the tribes by 1 and decimates the");
            this->boardModel->printMessage("farm and all wonders in the active region.");
            this->boardModel->printMessage(" ");

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING))
            {
                this->boardModel->printMessage("Advance (ENGINEERING):");
                this->boardModel->printMessage("Cities hit by the VOLCANO cannot have their City AV reduced below 1.");
                this->boardModel->printMessage(" ");

                if(currentCityAV - decimateCityAV < 1)
                {
                    activeRegion->setCityAV(1);
                }
                else
                {
                    activeRegion->setCityAV(activeRegion->getCityAV() - decimateCityAV);
                }
            }
            else
            {
                activeRegion->setCityAV(activeRegion->getCityAV() - decimateCityAV);
            }

            activeRegion->decimateTribes(1);
            activeRegion->setFarm(false);
            activeRegion->decimateWonders();


        }
        else if(activeRegion->hasVolcano())
        {
            this->boardModel->printMessage(QString("The volcano in region %1 errupts violently.").arg(activeRegion->getRegion()));
            this->boardModel->printMessage(" ");

            int currentCityAV = activeRegion->getCityAV();
            bool hasFarm = activeRegion->hasFarm();
            int currentTribes = activeRegion->getTribes();
            bool hasForest = activeRegion->hasForest();
            bool hasDesert = activeRegion->hasDesert();
            bool hasWonders = activeRegion->hasWonders();

            activeRegion->setCityAV(0);
            activeRegion->setCity(false);
            activeRegion->setFarm(false);
            activeRegion->setTribes(0);
            activeRegion->setForest(false);
            activeRegion->setDesert(true);
            activeRegion->decimateWonders();

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING))
            {
                this->boardModel->printMessage("Advance (ENGINEERING):");
                this->boardModel->printMessage("Cities hit by the VOLCANO cannot be decimated and");
                this->boardModel->printMessage("instead have their City AV reduced to 1.");
                this->boardModel->printMessage(" ");
                activeRegion->setCity(true);
                activeRegion->setCityAV(1);
            }

            if(currentCityAV != activeRegion->getCityAV() ||
               hasFarm != activeRegion->hasFarm() ||
               currentTribes != activeRegion->getTribes() ||
               hasForest != activeRegion->hasForest() ||
               hasDesert != activeRegion->hasDesert() ||
               hasWonders != activeRegion->hasWonders())
            {
                this->boardModel->printMessage(QString("The erruption decimates the city, all the tribes,"));
                this->boardModel->printMessage("the farm, the forest and all wonders in the active region.");
                this->boardModel->printMessage(QString("As a result, a desert formed in region %1.").arg(activeRegion->getRegion()));
                this->boardModel->printMessage(" ");

                this->affectedRegions.insert(activeRegion->getRegion(), activeRegion);
            }
            else
            {
                this->boardModel->printMessage(QString("The volcano had no effect in region %1.").arg(activeRegion->getRegion()));
                this->boardModel->printMessage(" ");
            }
        }
        else
        {
            this->boardModel->printMessage(QString("A volcano forms in region %1.").arg(activeRegion->getRegion()));
            this->boardModel->printMessage("The volcano forming decimates the tribes by 1.");
            this->boardModel->printMessage(" ");

            activeRegion->setVolcano(true);
            activeRegion->decimateTribes(1);

            this->affectedRegions.insert(activeRegion->getRegion(), activeRegion);
        }
    }


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

    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
