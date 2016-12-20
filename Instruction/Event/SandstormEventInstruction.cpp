#include "SandstormEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

SandstormEventInstruction::SandstormEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0), desertsLeft(2)
{}

void SandstormEventInstruction::initInstruction()
{
    this->boardModel->printMessage("SANDSTORM:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *SandstormEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != -1)
    {
        return this;
    }

    if(button == Qt::RightButton && this->borderingRegions.contains(regionModel->getRegion()) && this->desertsLeft > 0)
    {
        this->desertsLeft--;
        this->boardModel->printMessage("The sandstorm spreads to region %1.");
        this->boardModel->printMessage("The farm and forest are decimated and a desert is created.");
        this->boardModel->printMessage(QString("The spreads to %1 more region(s).").arg(this->desertsLeft));
        this->boardModel->printMessage(" ");

        regionModel->setFarm(false);
        regionModel->setForest(false);
        regionModel->setDesert(true);

        if(this->desertsLeft == 0)
        {
            return this->endEvent();
        }
    }

    return this;
}

Instruction *SandstormEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;
        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step = -1;

        RegionModel *activeRegion = this->boardModel->refActiveRegion();

        if(activeRegion->hasDesert())
        {
            this->boardModel->printMessage("The desert in the active region amplifies the sandstorm and");
            this->boardModel->printMessage("makes it spread to 2 bordering regions.");
            this->boardModel->printMessage(" ");

            this->borderingRegions = this->boardModel->getAdjacentRegions(activeRegion->getRegion());

            if(this->borderingRegions.isEmpty())
            {
                this->boardModel->printMessage("But, there are no bordering regions to spread to.");
            }
            else if(this->borderingRegions.size() == 1)
            {
                this->boardModel->printMessage("But, there is only one bordering region to spread to.");
                this->boardModel->printMessage("The sand storm will only affect that region.");
            }
            else if(this->borderingRegions.size() == 2)
            {
                this->boardModel->printMessage("There are exactly two bordering regions.");
                this->boardModel->printMessage("Therefore, the sandstorm spreads to those regions.");
            }
            else
            {
                this->boardModel->printMessage("Choose two regions bordering on the active region where the");
                this->boardModel->printMessage("sandstorm is spreading to.");
                this->boardModel->printMessage("The spreading sandstorm decimates any farms and forest in the chosen regions");
                this->boardModel->printMessage("as well as create a desert.");
                this->boardModel->printMessage(" ");
                return this;
            }

            this->boardModel->printMessage(" ");

            this->desertsLeft = 0;
            foreach(RegionModel *regionModel, this->borderingRegions.values())
            {
                this->boardModel->printMessage("The sandstorm spreads to region %1.");
                this->boardModel->printMessage("The farm and forest are decimated and a desert is created.");
                this->boardModel->printMessage(" ");
                regionModel->setFarm(false);
                regionModel->setForest(false);
                regionModel->setDesert(true);
            }
        }
        else
        {
            this->boardModel->printMessage("The sandstorm decimates farms and forest in the active region and");
            this->boardModel->printMessage("creates a desert in place of them.");
            this->boardModel->printMessage(" ");
            this->desertsLeft = 0;
            activeRegion->setFarm(false);
            activeRegion->setForest(false);
            activeRegion->setDesert(true);
        }
    }

    if(this->step == -1 && this->desertsLeft > 0)
    {
        return this;
    }

    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}



