#include "InitialForestInstruction.hpp"

InitialForestInstruction::InitialForestInstruction(BoardModel *boardModel, QObject *parent)
    : Instruction(boardModel, parent)
{}

Instruction *InitialForestInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(regionModel != NULL)
        {
            regionModel->toggleForest();
        }
    }

    return this;
}

Instruction *InitialForestInstruction::triggerDone()
{
    if(this->boardModel->getForestCount() == 5)
    {
        this->boardModel->sendMessage(" ");
        QMap<int, RegionModel *> regions = this->boardModel->getRegions();
        QMap<int, RegionModel *> mRegions = this->boardModel->getMountainRegions();
        QMap<int, RegionModel *> fRegions = this->boardModel->getForestRegions();

        foreach(int key, mRegions.keys())
        {
            regions.remove(key);
        }

        foreach(int key, fRegions.keys())
        {
            regions.remove(key);
        }

        if(regions.isEmpty())
        {
            this->boardModel->sendMessage("No regions without mountain and/or forest found to place a desert.");
            this->boardModel->sendMessage("Place ONE desert into one region without a forest.");
            this->boardModel->sendMessage("When you are done, press Done...");
        }
        else
        {
            this->boardModel->sendMessage("Placing deserts into regions where there are no Mountain and no Forest...");
            foreach(RegionModel *regionModel, regions)
            {
                regionModel->setDesert();
            }
        }

        return this;
    }
    else
    {
        if(this->boardModel->getForestCount() < 5)
        {
            this->boardModel->sendMessage("Not enough forest placed.");
        }
        else
        {
            this->boardModel->sendMessage("Too many forest placed.");
        }
    }

    return this;
}
