#include "VolcanoEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

VolcanoEventInstruction::VolcanoEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void VolcanoEventInstruction::initInstruction()
{
    this->boardModel->printMessage("VOLCANO:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Advance (ENGINEERING):");
    this->boardModel->printMessage("Cities hit by the VOLCANO which then have their CityAV");
    this->boardModel->printMessage("reduced below 1, instead have their City AV set to 1.");
    this->boardModel->printMessage(" ");
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
            this->boardModel->printMessage("The mountain in the active region becomes an volcano and errupts.");
            this->boardModel->printMessage(" ");

            activeRegion->setMountain(false);
            activeRegion->setVolcano(true);

            int currentCityAV = activeRegion->getCityAV();
            int savedCityAV = 0;

            activeRegion->decreaseCityAV(2);

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING) && activeRegion->getCityAV() < 1)
            {
                savedCityAV = 1 - (currentCityAV - 2);
                activeRegion->setCityAV(1);
            }

            activeRegion->decimateTribes(1);
            activeRegion->setFarm(false);
            activeRegion->decimateWonders();

            this->boardModel->printMessage(QString("The erruption decreased the City AV by %1,").arg(2 - savedCityAV));
            this->boardModel->printMessage("decimated the tribes by 1 and decimated the");
            this->boardModel->printMessage("farm and all wonders in the active region.");
        }
        else if(activeRegion->hasVolcano())
        {
            this->boardModel->printMessage("The volcano in the active region errupts violently.");
            this->boardModel->printMessage(" ");

            int setCityAV = 0;

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING))
            {
                setCityAV = 1;
            }

            activeRegion->setCityAV(setCityAV);
            activeRegion->setCity(false);
            activeRegion->setFarm(false);
            activeRegion->setTribes(0);
            activeRegion->setForest(false);
            activeRegion->setDesert(true);
            activeRegion->decimateWonders();

            this->boardModel->printMessage(QString("The erruption set the City AV to %1 and decimated").arg(setCityAV));
            this->boardModel->printMessage("all the tribes, the farm, the forest and all wonders");
            this->boardModel->printMessage("in the active region.");
            this->boardModel->printMessage("As a result, a desert formed in the active region.");
        }
        else
        {
            this->boardModel->printMessage("A volcano forms in the plains of the active region.");
            this->boardModel->printMessage(" ");

            activeRegion->setVolcano(true);
            activeRegion->decimateTribes(1);

            this->boardModel->printMessage("The volcano forming decimated the tribes by 1.");
        }
    }

    this->boardModel->printMessage(" ");
    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
