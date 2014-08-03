#include "ChooseRegionInstruction.hpp"

#include "Common.hpp"

ChooseRegionInstruction::ChooseRegionInstruction(BoardModel *boardModel, int currentRegion, int maxRegions, QObject *parent)
    : Instruction(boardModel, parent), currentRegion(currentRegion), maxRegions(maxRegions)
{}

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
    QSet<HexModel *> region = this->boardModel->getRegions().value(this->currentRegion);
    if(region.size() > 1)
    {
        int i = Common::randomWithSeed()%region.size();
        region.values().at(i)->setRegionNumberShown(true);

        if(currentRegion == maxRegions)
        {
            this->boardModel->sendMessage("Done creating the world!");
            return this; // TODO: Here go to next instruction.
        }
        else
        {
            this->boardModel->sendMessage(QString("Region %1 out of %2.").
                                         arg(currentRegion+1).arg(maxRegions));
            this->deleteLater();
            return new ChooseRegionInstruction(this->boardModel, currentRegion+1, maxRegions);
        }
    }

    return this;
}
