#include "PopulationGrowthInstruction.hpp"

#include "Instruction/MoveTribesInstruction.hpp"

PopulationGrowthInstruction::PopulationGrowthInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void PopulationGrowthInstruction::initInstruction()
{
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("POPULATION GROWTH:");
    this->boardModel->sendMessage("Added 1 tribe to any region with at least 1 Tribe on it.");
    this->boardModel->populationGrowth();
    this->boardModel->sendMessage(" ");
    if(this->boardModel->getTribeCount() < 3)
    {
        this->boardModel->sendMessage("The EMPIRE holds less than 3 tribes.");
        this->boardModel->sendMessage("Put as many tribes into any region until there are 3 tribes.");
        this->boardModel->sendMessage("When you are done, press done.");
        this->boardModel->sendMessage(" ");
    }
    else
    {
        this->boardModel->sendMessage("Press done to continue.");
        this->boardModel->sendMessage(" ");
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
        this->boardModel->sendMessage("There are still less than 3 tribes in the EMPIRE.");
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
