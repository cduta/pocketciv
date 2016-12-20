#include "ChooseRegionInstruction.hpp"

#include "Common.hpp"
#include "ChooseFrontierInstruction.hpp"

ChooseRegionInstruction::ChooseRegionInstruction(BoardModel *boardModel, int currentRegion, int maxRegions)
    : Instruction(), boardModel(boardModel), currentRegion(currentRegion), maxRegions(maxRegions)
{}

void ChooseRegionInstruction::initInstruction()
{
    this->boardModel->printMessage(QString("Region %1 out of %2.").
                                 arg(currentRegion).arg(maxRegions));
}

Instruction *ChooseRegionInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        if(this->boardModel->refHexModel(x,y)->getRegion() == -1 || this->boardModel->refHexModel(x,y)->getRegion() == this->currentRegion)
        {
            this->boardModel->toggleHexToRegion(this->currentRegion, x, y);
        }
    }

    return this;
}

Instruction *ChooseRegionInstruction::triggerDone()
{
    QSet<HexModel *> region = this->boardModel->getRegionHexes().value(this->currentRegion);
    if(region.size() > 1)
    {
        int i = Common::randomSeed()%region.size();
        region.values().at(i)->setRegionNumberShown(true);

        if(currentRegion == maxRegions)
        {
            Instruction *next = new ChooseFrontierInstruction(this->boardModel);
            next->initInstruction();
            return next;
        }
        else
        {
            Instruction *next = new ChooseRegionInstruction(this->boardModel, currentRegion+1, maxRegions);
            next->initInstruction();
            return next;
        }
    }
    else
    {
        this->boardModel->printMessage("The current region you created is not 2 or more hexes in size.");
    }

    return this;
}
