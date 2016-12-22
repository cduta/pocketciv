#include "CivilWarEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

CivilWarEventInstruction::CivilWarEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event)
{
    this->nextInstruction->setKeepInstruction(true);
    this->colleteralDamage = 0;
    this->totalTribes = 0;
    this->step = 0;
    this->cityAVLost = 2;
}

void CivilWarEventInstruction::initInstruction()
{
    this->boardModel->printMessage("CIVIL WAR:");
    this->boardModel->printMessage(" ");
    if(this->boardModel->getAdvancesAquired().contains(AdvanceModel::ARCHITECTURE))
    {
        this->cityAVLost = 1;
        this->boardModel->printMessage("Advance (ARCHITECTURE):");
        this->boardModel->printMessage("The city AV of the active region and bordering regions are reduced by 1.");
        this->boardModel->printMessage("Any regions of those with a city are thereby affected regions.");
    }
    else
    {
        this->cityAVLost = 2;
        this->boardModel->printMessage("The city AV of the active region and bordering regions are reduced by 2.");
        this->boardModel->printMessage("Any regions of those with a city are thereby affected regions.");
    }
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *CivilWarEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != -1)
    {
        return this;
    }

    if(button == Qt::LeftButton &&
       this->colleteralDamage > 0 &&
       regionModel->getTribes() > 0 &&
       affectedRegions.contains(regionModel->getRegion()))
    {
        regionModel->setTribes(regionModel->getTribes()-1);
        this->colleteralDamage--;

        if(this->colleteralDamage == 0)
        {
            this->boardModel->printMessage("All colleteral damage has been distributed.");
            return this->endEvent();
        }
        else
        {
            this->boardModel->printMessage(QString("Decimated one Tribe in Region %1. Colleteral damage left: %2")
                                          .arg(regionModel->getRegion())
                                          .arg(this->colleteralDamage));
        }
    }

    return this;
}

Instruction *CivilWarEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;
        this->drawActiveRegion();

        RegionModel *activeRegion = this->boardModel->refActiveRegion();
        QMap<int, RegionModel *> possibleRegions;
        possibleRegions = this->boardModel->getAdjacentRegions(activeRegion->getRegion());
        possibleRegions.insert(activeRegion->getRegion(), activeRegion);

        foreach(int region, possibleRegions.keys())
        {
            RegionModel *regionModel = possibleRegions.value(region);
            if(regionModel->hasCity())
            {
                if(this->boardModel->hasAdvanceAquired(AdvanceModel::CIVIL_SERVICE) && regionModel->getCityAV() == 1)
                {
                    this->boardModel->printMessage("Advance (CIVIL SERVICE):");
                    this->boardModel->printMessage("The City AV can't be reduced below 1.");
                }
                else
                {
                    regionModel->decreaseCityAV(this->cityAVLost);
                    regionModel->decimateZeroAVCity();
                    this->affectedRegions.insert(region, regionModel);
                }
            }
        }
        this->boardModel->printMessage(" ");

        if(this->affectedRegions.isEmpty())
        {
            this->boardModel->printMessage("But, none of the regions had a city.");
            this->boardModel->unsetActiveRegion();
            return this->endEvent();
        }
        else
        {
            this->boardModel->printMessage(QString("The affected regions are %1.")
                                          .arg(Common::listUpRegions(this->affectedRegions.values())));
        }
        this->boardModel->printMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step = 2;

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::LAW))
        {
            this->boardModel->printMessage("Advance (LAW):");
            this->boardModel->printMessage("The CIVIL WAR is less severe because of this advance.");
            this->boardModel->printMessage(" ");

            this->colleteralDamage = this->boardModel->drawCard()->getShapeNumbers().value(Event::GREEN_SQUARE, 0);
        }
        else
        {
            this->colleteralDamage = this->boardModel->drawCard()->getShapeNumbers().value(Event::BLUE_HEXAGON, 0);
        }

        foreach(RegionModel *regionModel, this->affectedRegions.values())
        {
            this->totalTribes += regionModel->getTribes();
        }

        this->boardModel->printMessage("The affected regions have their tribes reduced.");
        this->boardModel->printMessage(QString("The total amount of tribes to be reduced is the colleteral damage."));

        // NOTE: Before MEDITATION!
        if(this->boardModel->getAdvancesAquired().contains(AdvanceModel::ARTS))
        {
            this->reduceColleteralDamageBy(2);
            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("Advance (ARTS):");
            this->boardModel->printMessage("The colleteral damage is reduced by 2.");
        }

        this->boardModel->printMessage(" ");
        this->boardModel->printMessage(QString("The colleteral damage is %1.").arg(this->colleteralDamage));
        this->boardModel->printMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 2 && this->colleteralDamage > 0)
    {
        this->step = -1;

        if(this->totalTribes > this->colleteralDamage)
        {
            this->boardModel->printMessage("Distribute all the colleteral damage in the affected regions with tribes.");
            this->boardModel->printMessage("Reduce a tribe for each colleteral damage distributed in the affected regions.");
            this->boardModel->printMessage(" ");

            return this;
        }
        else
        {
            foreach(RegionModel *RegionModel, this->affectedRegions.values())
            {
                RegionModel->setTribes(0);
            }
            this->colleteralDamage = 0;

            this->boardModel->printMessage("The total amount of tribes are less or equal than the colleteral damage.");
            this->boardModel->printMessage("Therefore, ALL tribes are reduced.");
        }
    }

    if(this->step == -1 && this->colleteralDamage == 0)
    {
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }

    return this;
}

void CivilWarEventInstruction::reduceColleteralDamageBy(int value)
{
    this->colleteralDamage -= value;

    if(this->colleteralDamage < 0)
    {
        this->colleteralDamage = 0;
    }

    return;
}
