#include "TribalWarEventInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

TribalWarEventInstruction::TribalWarEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0), warringTribes(0), borderingRegionsLeft(2)
{}

void TribalWarEventInstruction::initInstruction()
{
    this->boardModel->printMessage("TRIBAL WAR:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *TribalWarEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != -1)
    {
        return this;
    }

    if(button == Qt::RightButton && this->borderingRegions.contains(regionModel->getRegion()) && this->borderingRegionsLeft > 0)
    {
        this->borderingRegionsLeft--;
        regionModel->decimateTribes(this->warringTribes);
        this->boardModel->printMessage(QString("The tribes of region %1 are reduced by %2.")
                                      .arg(regionModel->getRegion())
                                      .arg(this->warringTribes));

        if(this->borderingRegionsLeft == 0)
        {
            this->boardModel->refActiveRegion()->decimateTribes(3);
            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("Reduce 3 tribes in the active region because of the tribal war.");
            this->boardModel->printMessage(" ");
            return this->endEvent();
        }
    }

    return this;
}

Instruction *TribalWarEventInstruction::triggerDone()
{
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::GOVERNMENT))
    {
        this->boardModel->printMessage("Advance (GOVERNMENT):");
        this->boardModel->printMessage("Through this advance, this event is skipped.");
        this->boardModel->printMessage(" ");
        return this->endEvent();
    }

    if(this->step == 0)
    {
        this->step++;
        this->drawActiveRegion();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step = -1;

        RegionModel *activeRegion = this->boardModel->refActiveRegion();
        int tribes = activeRegion->getTribes();

        this->boardModel->printMessage(QString("The active region is region %1.").arg(activeRegion->getRegion()));

        if(tribes == 0)
        {
            this->boardModel->printMessage(QString("Region %1 has NO tribes.").arg(activeRegion->getRegion()));
            this->boardModel->printMessage(" ");
            this->boardModel->unsetActiveRegion();
            return this->endEvent();
        }
        else
        {
            this->warringTribes = tribes*2;
            this->boardModel->printMessage(QString("Region %1 has %2 warring tribes.").arg(activeRegion->getRegion()).arg(this->warringTribes));
        }

        QMap<int, RegionModel *> possibleRegions = this->boardModel->getAdjacentRegions(activeRegion->getRegion());

        foreach(int region, possibleRegions.keys())
        {
            RegionModel *regionModel = possibleRegions.value(region);

            if(regionModel->hasTribes())
            {
                this->borderingRegions.insert(region, regionModel);
            }
        }

        this->boardModel->printMessage(QString("Choose 2 of the regions with tribes bordering on region %1.").arg(activeRegion->getRegion()));
        this->boardModel->printMessage(QString("In those regions, the tribes are reduced by %1, the amount of warring tribes.").arg(this->warringTribes));
        this->boardModel->printMessage(" ");
        this->borderingRegionsLeft = 2;

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::MUSIC))
        {
            this->boardModel->printMessage("Advance (MUSIC):");
            this->boardModel->printMessage("Instead of 2, choose one bordering region with tribes");
            this->boardModel->printMessage(" ");
            this->borderingRegionsLeft = 1;
        }

        this->boardModel->printMessage(" ");

        if(this->borderingRegions.size() == 0)
        {
            this->boardModel->printMessage("There are no bordering regions with tribes");
            this->boardModel->printMessage("which ends the tribal war.");
            this->boardModel->printMessage(" ");
        }
        else if(this->borderingRegions.size() == 1 && this->borderingRegionsLeft >= 1)
        {
            this->boardModel->printMessage("There is only one bordering region with tribes.");
            this->borderingRegions.values().at(0)->decimateTribes(this->warringTribes);
            this->boardModel->printMessage(QString("The tribes of region %1 are reduced by %2.")
                                          .arg(this->borderingRegions.values().at(0)->getRegion())
                                          .arg(this->warringTribes));
            this->boardModel->printMessage(" ");
        }
        else if(this->borderingRegionsLeft == 2 && this->borderingRegionsLeft >= 2)
        {
            this->boardModel->printMessage("There are exactly two bordering regions with tribes.");
            this->borderingRegions.values().at(0)->decimateTribes(this->warringTribes);
            this->boardModel->printMessage(QString("The tribes of region %1 are reduced by %2.")
                                          .arg(this->borderingRegions.values().at(0)->getRegion())
                                          .arg(this->warringTribes));
            this->borderingRegions.values().at(1)->decimateTribes(this->warringTribes);
            this->boardModel->printMessage(QString("The tribes of region %1 are reduced by %2.")
                                          .arg(this->borderingRegions.values().at(1)->getRegion())
                                          .arg(this->warringTribes));
            this->boardModel->printMessage(" ");
        }
        else
        {
            this->boardModel->printMessage(QString("Possible regions to choose are regions $1.").arg(Common::listUpRegions(possibleRegions.values())));
            this->boardModel->printMessage(" ");
            return this;
        }
    }

    if(this->step == -1 && this->borderingRegionsLeft > 0)
    {
        return this;
    }

    int activeRegionTribalReduce = 3;

    this->boardModel->printMessage(QString("Decimate 3 tribes in region %1 because of the tribal war.").arg(this->boardModel->refActiveRegion()->getRegion()));
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MUSIC))
    {
        this->boardModel->printMessage("Advance (MUSIC):");
        this->boardModel->printMessage(QString("Instead of decimating 3 tribes, decimte one tribe.").arg(this->boardModel->refActiveRegion()->getRegion()));
        this->boardModel->printMessage(" ");

        activeRegionTribalReduce = 1;
    }

    this->boardModel->refActiveRegion()->decimateTribes(activeRegionTribalReduce);
    this->boardModel->unsetActiveRegion();
    return this->endEvent();
}
