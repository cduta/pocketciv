#include "PlaceInitialTribes.hpp"

#include "Instruction/MoveTribesInstruction.hpp"

PlaceInitialTribes::PlaceInitialTribes(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void PlaceInitialTribes::initInstruction()
{
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Place 3 Tribes into any amount of regions.");
    this->boardModel->sendMessage("When you are done, press Done....");
}

Instruction *PlaceInitialTribes::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        if(this->boardModel->getTribeCount() < 3)
        {
            RegionModel *region = this->boardModel->refRegionModel(x,y);
            if(region != NULL)
            {
                region->setTribes(region->getTribes()+1);
            }
        }
    }
    else if(button == Qt::RightButton)
    {
        if(this->boardModel->getTribeCount() > 0)
        {
            RegionModel *region = this->boardModel->refRegionModel(x,y);
            if(region != NULL && region->getTribes() > 0)
            {
                region->setTribes(region->getTribes()-1);
            }
        }
    }

    return this;
}

Instruction *PlaceInitialTribes::triggerDone()
{
    if(this->boardModel->getTribeCount() == 3)
    {
        this->boardModel->sendMessage("Done creating the world!");
        this->boardModel->clearMessages();
        this->boardModel->sendMessage("Discarding 3 event cards...");
        this->boardModel->drawCard(false);
        this->boardModel->drawCard(false);
        this->boardModel->drawCard(false);
        this->boardModel->sendMessage("The Game begins...");
        this->boardModel->sendMessage("Population Growth:");
        this->boardModel->sendMessage("Added 1 tribe to any region with at least 1 Tribe on it.");
        this->boardModel->populationGrowth();
        this->boardModel->sendMessage(" ");
        Instruction *next = new MoveTribesInstruction(this->boardModel);
        next->initInstruction();
        return next;
    }
    else
    {
        this->boardModel->sendMessage("You did not put down 3 tribes yet.");
    }

    return this;
}

