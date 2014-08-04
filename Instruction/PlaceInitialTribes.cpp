#include "PlaceInitialTribes.hpp"

PlaceInitialTribes::PlaceInitialTribes(BoardModel *boardModel, QObject *parent)
    : Instruction(boardModel, parent)
{}

Instruction *PlaceInitialTribes::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        if(this->boardModel->getTribeCount() < 3)
        {
            RegionModel *region = this->boardModel->refRegionModel(x,y);
            region->setTribes(region->getTribes()+1);
        }
    }
    else if(button == Qt::RightButton)
    {
        if(this->boardModel->getTribeCount() > 0)
        {
            RegionModel *region = this->boardModel->refRegionModel(x,y);
            if(region->getTribes() > 0)
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
        this->boardModel->sendMessage("The Game begins...");
        return this;
    }
    else
    {
        this->boardModel->sendMessage("You did not put down 3 tribes yet.");
    }

    return this;
}

