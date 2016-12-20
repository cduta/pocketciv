#include "TsunamiInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

TsunamiInstruction::TsunamiInstruction(BoardModel *boardModel, RegionModel *activeRegion, Instruction *nextInstruction, const Event *firstEvent)
    : Instruction(), firstEvent(firstEvent), damagePerTribe(1), damagePerCityAV(2), damagePerWonder(3)
{
    this->step = 0;
    this->tsunamiDamage = 0;
    this->boardModel = boardModel;
    this->activeRegion = activeRegion;
    this->nextInstruction = nextInstruction;
}

void TsunamiInstruction::initInstruction()
{
    this->boardModel->printMessage(QString("TSUNAMI:"));
    this->boardModel->printMessage(" ");

    this->affectedRegions.append(this->activeRegion);

    QMap<int, RegionModel *> regions = this->boardModel->getAdjacentRegions(this->activeRegion->getRegion());

    foreach(int region, regions.keys())
    {
        RegionModel *regionModel = regions.value(region);
        if(this->boardModel->bordersOnSea(regionModel->getRegion()))
        {
            this->affectedRegions.append(regionModel);
        }
    }

    this->boardModel->printMessage(QString("Regions affected by the tsunami is the active region (region %1) and")
                                  .arg(this->activeRegion->getRegion()));
    this->boardModel->printMessage(QString("regions bordering on the active region and the sea."));
    this->boardModel->printMessage(QString("The affected region(s): %1.")
                                  .arg(Common::listUpRegions(this->affectedRegions)));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *TsunamiInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;
        this->tsunamiDamage = this->boardModel->drawCard()->getShapeNumberSum(this->firstEvent->getShapeNumberAmounts());
        this->boardModel->printMessage(" ");
        this->boardModel->printMessage(QString("The damage in each affected region caused by the tsunami is %1.")
                                      .arg(this->tsunamiDamage));

        QString advanceInfo = "";
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::CIVIL_SERVICE))
        {
            advanceInfo = " [Advance (CIVIL SERVICE)] ";
            this->damagePerTribe = 3;
            this->damagePerCityAV = 5;
        }

        this->boardModel->printMessage(QString("For every %1 damage%2, one tribe will decimated.")
                                      .arg(this->damagePerTribe)
                                      .arg(advanceInfo));
        this->boardModel->printMessage(QString("With no tribes left, for every %1 damage%2, one City AV will be reduced.")
                                      .arg(this->damagePerCityAV)
                                      .arg(advanceInfo));

        advanceInfo = "";
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING))
        {
            advanceInfo = " [Advance (ENGINEERING)] ";
            this->damagePerWonder = 10;
        }

        this->boardModel->printMessage(QString("With the city destroyed, for every %1 damage%2, one Wonder will be decimated.")
                                      .arg(this->damagePerWonder)
                                      .arg(advanceInfo));
        this->boardModel->printMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(!this->affectedRegions.isEmpty())
    {
        RegionModel *affectedRegion = this->affectedRegions.takeFirst();
        int currentDamage = this->tsunamiDamage;
        int totalTribeDamage = 0;
        int totalCityDamage = 0;
        //TODO: Add Wonder decimation for 3 damage.

        while(currentDamage > 0)
        {
            if(affectedRegion->hasTribes())
            {
                if(currentDamage < this->damagePerTribe)
                {
                    currentDamage = 0;
                }
                else
                {
                    affectedRegion->decimateTribes(1);
                    totalTribeDamage++;
                    currentDamage -= this->damagePerTribe;
                }
            }
            else if(affectedRegion->hasCity())
            {
                if(currentDamage < this->damagePerCityAV)
                {
                    currentDamage = 0;
                }
                else
                {
                    if(affectedRegion->getCityAV() > 0)
                    {
                        affectedRegion->decreaseCityAV(1);
                        totalCityDamage++;
                        currentDamage -= this->damagePerCityAV;
                    }
                    affectedRegion->decimateZeroAVCity();
                }
            }
            else
            {
                currentDamage = 0;
            }
        }

        this->boardModel->printMessage(QString("In region %1, the tsunami results in the following losses.")
                                      .arg(affectedRegion->getRegion()));
        this->boardModel->printMessage(QString("Tribes: %1")
                                      .arg(totalTribeDamage));
        this->boardModel->printMessage(QString("CityAV: %1")
                                      .arg(totalCityDamage));

        this->boardModel->printMessage(" ");
        this->boardModel->printMessage(QString("Affected regions left: %1.")
                                      .arg(Common::listUpRegions(this->affectedRegions)));
        this->boardModel->printMessage("Press Done to continue.");
        this->boardModel->printMessage(" ");
        return this;
    }

    this->boardModel->unsetActiveRegion();
    this->boardModel->printMessage("The Tsunami has subsided and the event ends.");
    this->boardModel->printMessage(" ");
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
