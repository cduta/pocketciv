#include "UprisingEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

UprisingEventInstruction::UprisingEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void UprisingEventInstruction::initInstruction()
{
    this->boardModel->sendMessage("UPRISING:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
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

        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("An uprising decreased the City AV by 2, decimated the tribes by 2 and");
        this->boardModel->sendMessage("decimated the farm in the active region.");
    }

    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
