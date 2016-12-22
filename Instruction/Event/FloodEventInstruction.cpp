#include "FloodEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Instruction/TsunamiInstruction.hpp"

FloodEventInstruction::FloodEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{}

void FloodEventInstruction::initInstruction()
{
    this->boardModel->printMessage("FLOOD:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(" ");

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
        this->boardModel->printMessage("The active region borders on the sea.");
        this->boardModel->printMessage("Therefore, the flood escalated into a tsunami.");
        this->boardModel->printMessage(" ");

        Instruction *instruction = new TsunamiInstruction(this->boardModel, activeRegion, this->nextInstruction, this->event);
        instruction->initInstruction();
        return instruction;
    }
    else
    {
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::IRRIGATION))
        {
            this->boardModel->printMessage("Advance (IRRIGATION):");
            this->boardModel->printMessage(QString("The flood decimates 2 tribes in Region %1.").arg(activeRegion->getRegion()));
            this->boardModel->printMessage(" ");
        }
        else
        {
            this->boardModel->printMessage("The flood decimates 2 tribes, the farm and");
            this->boardModel->printMessage(QString("reduces the City AV by 1 in Region %1.").arg(activeRegion->getRegion()));
            this->boardModel->printMessage(" ");
        }

        activeRegion->setFarm(false);
        activeRegion->decreaseCityAV(1);

        activeRegion->decimateTribes(2);

        if(!activeRegion->hasForest())
        {
            activeRegion->setForest(true);
            this->boardModel->printMessage("After the flood subsides, a forest grows in the active region.");
            this->boardModel->printMessage(" ");
        }
    }

    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}



