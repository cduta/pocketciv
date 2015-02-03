#include "VolcanoEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

VolcanoEventInstruction::VolcanoEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void VolcanoEventInstruction::initInstruction()
{
    this->boardModel->sendMessage("VOLCANO:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
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
            this->boardModel->sendMessage("The mountain in the active region becomes an volcano and errupts.");
            this->boardModel->sendMessage(" ");

            activeRegion->setMountain(false);
            activeRegion->setVolcano(true);
            activeRegion->decreaseCityAV(2);
            activeRegion->decimateTribes(1);
            activeRegion->setFarm(false);
            activeRegion->decimateWonders();

            this->boardModel->sendMessage("The erruption decreased the City AV by 2 and decimated the tribes by 1 and");
            this->boardModel->sendMessage("decimated the farm and all wonders in the active region.");
        }
        else if(activeRegion->hasVolcano())
        {
            this->boardModel->sendMessage("The volcano in the active region errupts violently.");
            this->boardModel->sendMessage(" ");

            activeRegion->setCityAV(0);
            activeRegion->setCity(false);
            activeRegion->setFarm(false);
            activeRegion->setTribes(0);
            activeRegion->setForest(false);
            activeRegion->setDesert(true);
            activeRegion->decimateWonders();

            this->boardModel->sendMessage("The violent erruption decimated the city, all the tribes, the farm,");
            this->boardModel->sendMessage("the forest and all wonders in the active region.");
            this->boardModel->sendMessage("As a result, a desert formed in the active region.");
        }
        else
        {
            this->boardModel->sendMessage("A volcano forms in the plains of the active region.");
            this->boardModel->sendMessage(" ");

            activeRegion->setVolcano(true);
            activeRegion->decimateTribes(1);

            this->boardModel->sendMessage("The volcano forming decimated the tribes by 1.");
        }
    }

    this->boardModel->sendMessage(" ");
    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
