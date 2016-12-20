#include "VolcanoEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

VolcanoEventInstruction::VolcanoEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void VolcanoEventInstruction::initInstruction()
{
    this->boardModel->printMessage("VOLCANO:");
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
            activeRegion->decreaseCityAV(2);
            activeRegion->decimateTribes(1);
            activeRegion->setFarm(false);
            activeRegion->decimateWonders();

            this->boardModel->printMessage("The erruption decreased the City AV by 2 and decimated the tribes by 1 and");
            this->boardModel->printMessage("decimated the farm and all wonders in the active region.");
        }
        else if(activeRegion->hasVolcano())
        {
            this->boardModel->printMessage("The volcano in the active region errupts violently.");
            this->boardModel->printMessage(" ");

            activeRegion->setCityAV(0);
            activeRegion->setCity(false);
            activeRegion->setFarm(false);
            activeRegion->setTribes(0);
            activeRegion->setForest(false);
            activeRegion->setDesert(true);
            activeRegion->decimateWonders();

            this->boardModel->printMessage("The violent erruption decimated the city, all the tribes, the farm,");
            this->boardModel->printMessage("the forest and all wonders in the active region.");
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
