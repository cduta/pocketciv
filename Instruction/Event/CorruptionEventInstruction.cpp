#include "CorruptionEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

CorruptionEventInstruction::CorruptionEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event)
{
    this->step = 0;
    this->corruption = 0;
    this->cityAVTotal = 0;
}

void CorruptionEventInstruction::initInstruction()
{
    this->boardModel->printMessage("CORRUPTION:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *CorruptionEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != -1)
    {
        return this;
    }

    if(button == Qt::LeftButton &&
       this->corruption > 0 &&
       regionModel->hasCity())
    {
        regionModel->decreaseCityAV(1);
        regionModel->decimateZeroAVCity();
        this->corruption--;

        if(this->corruption == 0)
        {
            this->boardModel->printMessage("All corruption has been distributed.");
            this->boardModel->printMessage(" ");
            return this->endCorruptionEvent();
        }
        else
        {
            if(regionModel->hasCity())
            {
                this->boardModel->printMessage(QString("Decreased one City AV in Region %1. Colleteral damage left: %2")
                                              .arg(regionModel->getRegion())
                                              .arg(this->corruption));
            }
            else
            {
                this->boardModel->printMessage(QString("Decimated City in Region %1. Colleteral damage left: %2")
                                              .arg(regionModel->getRegion())
                                              .arg(this->corruption));
            }
        }
    }

    return this;
}

Instruction *CorruptionEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;

        this->boardModel->printMessage("Distribute all the corruption in the regions with cities.");
        this->corruption += this->boardModel->drawCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
        this->boardModel->printMessage(" ");

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::GOVERNMENT))
        {
            this->boardModel->printMessage("Advance (GOVERNMENT):");
            this->boardModel->printMessage("Add an additonal 3 corruption.");
            this->boardModel->printMessage(" ");
        }

        // TODO: Literacy goes here.

        this->boardModel->printMessage(QString("The corruption is %1.").arg(this->corruption));

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step = -1;

        QMap<int, RegionModel *> regionMap = this->boardModel->getRegions();

        foreach(RegionModel *regionModel, regionMap.values())
        {
            this->cityAVTotal += regionModel->getCityAV();
        }

        if(this->cityAVTotal > this->corruption)
        {
            this->boardModel->printMessage("Reduce a tribe for each corruption distributed in the regions with cities.");
            this->boardModel->printMessage(" ");
        }
        else
        {
            this->boardModel->printMessage("There is more corruption than there are City AV in total.");
            this->boardModel->printMessage("Therefore, all cities are decimated.");
            this->boardModel->printMessage(" ");

            foreach(RegionModel *regionModel, regionMap.values())
            {
                regionModel->setCityAV(0);
                regionModel->decimateZeroAVCity();
            }
            this->corruption = 0;
        }

    }

    if(this->step == -1 && this->corruption == 0)
    {
        return this->endCorruptionEvent();
    }

    return this;
}

Instruction *CorruptionEventInstruction::endCorruptionEvent()
{
    this->boardModel->setGold(0);
    this->boardModel->printMessage("Coruption decimated all gold.");
    this->boardModel->printMessage(" ");
    return this->endEvent();
}
