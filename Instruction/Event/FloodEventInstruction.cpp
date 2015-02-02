#include "FloodEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Instruction/TsunamiInstruction.hpp"

FloodEventInstruction::FloodEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void FloodEventInstruction::initInstruction()
{
    this->boardModel->sendMessage("FLOOD:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press done to continue.");
    this->boardModel->sendMessage(" ");

    return;
}

Instruction *FloodEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;

        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    RegionModel *activeRegion = this->boardModel->refActiveRegion();

    if(this->boardModel->bordersOnSea(activeRegion->getRegion()))
    {
        this->boardModel->sendMessage("The active region borders on the sea.");
        this->boardModel->sendMessage("Therefore, the flood escalated into a tsunami.");
        this->boardModel->sendMessage(" ");

        Instruction *instruction = new TsunamiInstruction(this->boardModel, activeRegion, this->nextInstruction, this->event);
        instruction->initInstruction();
        return instruction;
    }
    else
    {
        activeRegion->decimateTribes(2);
        activeRegion->setFarm(false);
        activeRegion->decreaseCityAV(1);

        this->boardModel->sendMessage("The flood decimates 2 tribes, the farm and");
        this->boardModel->sendMessage("reduces the City AV by 1 in the active region.");
        this->boardModel->sendMessage(" ");

        if(!activeRegion->hasForest())
        {
            activeRegion->setForest(true);
            this->boardModel->sendMessage("After the flood subsides, a forest grows in the active region.");
            this->boardModel->sendMessage(" ");
        }
    }

    return this->endEvent();
}



