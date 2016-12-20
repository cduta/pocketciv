#include "UprisingEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

UprisingEventInstruction::UprisingEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void UprisingEventInstruction::initInstruction()
{
    this->boardModel->printMessage("UPRISING:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *UprisingEventInstruction::triggerDone()
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

        activeRegion->decreaseCityAV(2);
        activeRegion->decimateTribes(2);
        activeRegion->setFarm(false);

        this->boardModel->printMessage(" ");
        this->boardModel->printMessage("An uprising decreased the City AV by 2, decimated the tribes by 2 and");
        this->boardModel->printMessage("decimated the farm in the active region.");
    }

    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
