#include "PopulationGrowthInstruction.hpp"

#include "Instruction/MoveTribesInstruction.hpp"

PopulationGrowthInstruction::PopulationGrowthInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void PopulationGrowthInstruction::initInstruction()
{
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("POPULATION GROWTH:");
    this->boardModel->printMessage("Added 1 tribe to any region with at least 1 Tribe on it.");
    this->boardModel->populationGrowth();
    this->boardModel->printMessage(" ");
    if(this->boardModel->getTribeCount() < 3)
    {
        this->boardModel->printMessage("The EMPIRE holds less than 3 tribes.");
        this->boardModel->printMessage("Put as many tribes into any region until there are 3 tribes.");
        this->boardModel->printMessage("When you are done, press done.");
        this->boardModel->printMessage(" ");
    }
    else
    {
        this->boardModel->printMessage("Press done to continue.");
        this->boardModel->printMessage(" ");
    }
    return;
}

Instruction *PopulationGrowthInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    int tribesMoved = this->boardModel->getAllMovedTribes();
    int tribes = this->boardModel->getTribeCount();

    if(button == Qt::LeftButton)
    {
        if(tribesMoved + tribes < 3)
        {
            regionModel->addToMovedTribes(1);
        }
    }
    else if(button == Qt::RightButton)
    {
        if(regionModel->getMovedTribes() > 0)
        {
            regionModel->removeFromMovedTribes(1);
        }
    }

    return this;
}

Instruction *PopulationGrowthInstruction::triggerDone()
{
    if(this->boardModel->getTribeCount()+this->boardModel->getAllMovedTribes() < 3)
    {
        this->boardModel->printMessage("There are still less than 3 tribes in the EMPIRE.");
    }
    else
    {
        this->boardModel->mergeAllMovedTribes();
        Instruction *next = new MoveTribesInstruction(this->boardModel);
        next->initInstruction();
        return next;
    }

    return this;
}
