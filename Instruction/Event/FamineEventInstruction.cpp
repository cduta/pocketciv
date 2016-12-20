#include "FamineEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

FamineEventInstruction::FamineEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void FamineEventInstruction::initInstruction()
{
    this->boardModel->printMessage("FAMINE:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *FamineEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;

        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    this->boardModel->printMessage("Famine decimates all tribes, the farm and");
    this->boardModel->printMessage("reduces the City AV by 2 in the active region.");
    this->boardModel->printMessage("If the city AV is 0, the city is decimated.");

    RegionModel *activeRegion = this->boardModel->refActiveRegion();

    activeRegion->setTribes(0);
    activeRegion->setFarm(false);
    activeRegion->decreaseCityAV(2);
    activeRegion->decimateZeroAVCity();

    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
