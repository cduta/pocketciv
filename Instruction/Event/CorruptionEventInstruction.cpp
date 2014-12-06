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
    this->boardModel->sendMessage("CORRUPTION:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press done to continue.");
    this->boardModel->sendMessage(" ");
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
            this->boardModel->sendMessage("All corruption has been distributed.");
            this->boardModel->sendMessage(" ");
            return this->endCorruptionEvent();
        }
        else
        {
            if(regionModel->hasCity())
            {
                this->boardModel->sendMessage(QString("Decreased one City AV in Region %1. Colleteral damage left: %2")
                                              .arg(regionModel->getRegion())
                                              .arg(this->corruption));
            }
            else
            {
                this->boardModel->sendMessage(QString("Decimated City in Region %1. Colleteral damage left: %2")
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

        this->boardModel->sendMessage("Distribute all the corruption in the regions with cities.");
        this->corruption = this->boardModel->drawCard()->getShapeNumberSum(this->event->getShapeNumberAmounts());
        this->boardModel->sendMessage(QString("The corruption is %1.").arg(this->corruption));

        POKET_CIV_END_OF_ERA_CHECK
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
            this->boardModel->sendMessage("Reduce a tribe for each corruption distributed in the regions with cities.");
            this->boardModel->sendMessage(" ");
        }
        else
        {
            this->boardModel->sendMessage("There is more corruption than there are City AV in total.");
            this->boardModel->sendMessage("Therefore, all cities are decimated.");
            this->boardModel->sendMessage(" ");

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
    this->boardModel->sendMessage("Coruption decimated all gold.");
    this->boardModel->sendMessage(" ");
    return this->endEvent();
}
