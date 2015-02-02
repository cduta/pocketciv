#include "SandstormEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

SandstormEventInstruction::SandstormEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0), desertsLeft(2)
{}

void SandstormEventInstruction::initInstruction()
{
    this->boardModel->sendMessage("SANDSTORM:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
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
        this->boardModel->sendMessage("The sandstorm spreads to region %1.");
        this->boardModel->sendMessage("The farm and forest are decimated and a desert is created.");
        this->boardModel->sendMessage(QString("The spreads to %1 more region(s).").arg(this->desertsLeft));
        this->boardModel->sendMessage(" ");

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
            this->boardModel->sendMessage("The desert in the active region amplifies the sandstorm and");
            this->boardModel->sendMessage("makes it spread to 2 bordering regions.");
            this->boardModel->sendMessage(" ");

            this->borderingRegions = this->boardModel->getAdjacentRegions(activeRegion->getRegion());

            if(this->borderingRegions.isEmpty())
            {
                this->boardModel->sendMessage("But, there are no bordering regions to spread to.");
            }
            else if(this->borderingRegions.size() == 1)
            {
                this->boardModel->sendMessage("But, there is only one bordering region to spread to.");
                this->boardModel->sendMessage("The sand storm will only affect that region.");
            }
            else if(this->borderingRegions.size() == 2)
            {
                this->boardModel->sendMessage("There are exactly two bordering regions.");
                this->boardModel->sendMessage("Therefore, the sandstorm spreads to those regions.");
            }
            else
            {
                this->boardModel->sendMessage("Choose two regions bordering on the active region where the");
                this->boardModel->sendMessage("sandstorm is spreading to.");
                this->boardModel->sendMessage("The spreading sandstorm decimates any farms and forest in the chosen regions");
                this->boardModel->sendMessage("as well as create a desert.");
                this->boardModel->sendMessage(" ");
                return this;
            }

            this->boardModel->sendMessage(" ");

            this->desertsLeft = 0;
            foreach(RegionModel *regionModel, this->borderingRegions.values())
            {
                this->boardModel->sendMessage("The sandstorm spreads to region %1.");
                this->boardModel->sendMessage("The farm and forest are decimated and a desert is created.");
                this->boardModel->sendMessage(" ");
                regionModel->setFarm(false);
                regionModel->setForest(false);
                regionModel->setDesert(true);
            }
        }
        else
        {
            this->boardModel->sendMessage("The sandstorm decimates farms and forest in the active region and");
            this->boardModel->sendMessage("creates a desert in place of them.");
            this->boardModel->sendMessage(" ");
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

    return this->endEvent();
}



