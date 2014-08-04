#include "InitialForestInstruction.hpp"

#include "Instruction/SetInitialDesertInstruction.hpp"
#include "PlaceInitialTribes.hpp"

InitialForestInstruction::InitialForestInstruction(BoardModel *boardModel)
    : Instruction(boardModel)
{
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Place 5 forests on 5 different regions.");
    this->boardModel->sendMessage("When you are done, press Done...");
}

Instruction *InitialForestInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(regionModel != NULL)
        {
            int forestCount = this->boardModel->getForestCount();
            if(forestCount < 5)
            {
                regionModel->toggleForest();
            }
            else if(forestCount == 5)
            {
                regionModel->unsetForest();
            }
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
            return new SetInitialDesertInstruction(this->boardModel);
        }
        else
        {
            this->boardModel->sendMessage("Placing deserts into regions where there are no Mountain and no Forest...");
            foreach(RegionModel *regionModel, regions)
            {
                regionModel->setDesert();
            }
            return new PlaceInitialTribes(this->boardModel);
        }

        return this;
    }
    else
    {
        this->boardModel->sendMessage("Not enough forest placed.");
    }

    return this;
}
