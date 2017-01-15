#include "UprisingEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

UprisingEventInstruction::UprisingEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0)
{
}

void UprisingEventInstruction::initInstruction()
{
    this->boardModel->printMessage("UPRISING:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *UprisingEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != 2)
    {
        return this;
    }

    if(button == Qt::LeftButton && regionModel->hasCity())
    {
        regionModel->decreaseCityAV(1);
        regionModel->decimateZeroAVCity();

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::ORGANIZED_RELIGION))
        {
            regionModel->decimateTribes(2);
        }
        else
        {
            regionModel->decimateTribes(regionModel->getTribes());
        }

        this->boardModel->printMessage(QString("You chose region %1").arg(regionModel->getRegion()));
        this->boardModel->printMessage("Press Done to continue...");
        this->boardModel->printMessage(" ");

        this->step = 3;
    }

    return this;
}

Instruction *UprisingEventInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step++;
        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step++;
        RegionModel *activeRegion = this->boardModel->refActiveRegion();

        this->boardModel->printMessage("An uprising decreases the City AV by 2, decimates all tribes and");
        this->boardModel->printMessage(QString("decimated the farm in Region %1.").arg(activeRegion->getRegion()));
        this->boardModel->printMessage(" ");

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::ORGANIZED_RELIGION))
        {
            this->boardModel->printMessage("Advance (ORGANIZED RELIGION):");
            this->boardModel->printMessage("Instead of all tribes, decimate 2 tribes.");
            this->boardModel->printMessage(" ");
        }

        int decreaseCityAV = 2;

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::LAW))
        {
            this->boardModel->printMessage("Advance (LAW):");
            this->boardModel->printMessage("Have one less City AV reduced.");
            this->boardModel->printMessage(" ");
        }

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::MACHINING))
        {
            this->boardModel->printMessage("Advance (MACHINING):");
            this->boardModel->printMessage("Have one more City AV reduced.");
            this->boardModel->printMessage(" ");
        }

        activeRegion->decreaseCityAV(decreaseCityAV);

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::ORGANIZED_RELIGION))
        {
            activeRegion->decimateTribes(2);
        }
        else
        {
            activeRegion->decimateTribes(activeRegion->getTribes());
        }
        activeRegion->setFarm(false);

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::SLAVE_LABOR))
        {
            this->boardModel->printMessage("Advance (MACHINING):");
            this->boardModel->printMessage("Decimate the Farm in all Regions which have no City.");
            this->boardModel->printMessage(" ");

            QList<RegionModel *> farmDecimatedRegions;

            foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
            {
                if(!regionModel->hasCity() && regionModel->hasFarm())
                {
                    regionModel->setFarm(false);
                    farmDecimatedRegions.append(regionModel);
                }
            }

            this->boardModel->printMessage(QString("The regions affected by this are %1.").arg(Common::listUpRegions(farmDecimatedRegions)));
            this->boardModel->printMessage(" ");
        }
    }

    if(this->step == 2 && this->boardModel->hasAdvanceAquired(AdvanceModel::MILITARY))
    {
        this->step = -1;
        RegionModel *activeRegion = this->boardModel->refActiveRegion();

        this->boardModel->printMessage("Advance (MILITARY):");
        this->boardModel->printMessage("Choose a bordering region with a City.");
        this->boardModel->printMessage("Reduce its City AV by 1 and decimate all tribes.");
        this->boardModel->printMessage("If the City AV is 0, decimate the City.");
        this->boardModel->printMessage(" ");

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::ORGANIZED_RELIGION))
        {
            this->boardModel->printMessage("Advance (ORGANIZED RELIGION):");
            this->boardModel->printMessage("Instead of all tribes, decimate 2 tribes.");
            this->boardModel->printMessage(" ");
        }

        QMap<int, RegionModel *> borderingRegions = this->boardModel->getAdjacentRegions(activeRegion->getRegion());

        foreach(int region, borderingRegions.keys())
        {
            RegionModel *currentRegion = borderingRegions[region];

            if(currentRegion->hasCity())
            {
                this->possibleRegions.insert(region, currentRegion);
            }
        }

        if(!this->possibleRegions.isEmpty())
        {
            return this;
        }
        else
        {
            this->boardModel->printMessage("But there is no bordering region with a city.");
            this->boardModel->printMessage(" ");
        }
    }

    if(this->step != -1)
    {
        this->boardModel->unsetActiveRegion();
        return this->endEvent();
    }

    return this;
}
