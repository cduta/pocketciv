#include "TsunamiInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

TsunamiInstruction::TsunamiInstruction(BoardModel *boardModel, RegionModel *activeRegion, Instruction *nextInstruction, const Event *firstEvent)
    : Instruction(), firstEvent(firstEvent), damagePerTribe(1), cityDamage(2), wonderDamage(3)
{
    this->step = 0;
    this->tsunamiDamage = 0;
    this->boardModel = boardModel;
    this->activeRegion = activeRegion;
    this->nextInstruction = nextInstruction;
}

void TsunamiInstruction::initInstruction()
{
    this->boardModel->sendMessage(QString("TSUNAMI:"));
    this->boardModel->sendMessage(" ");

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

    this->boardModel->sendMessage(QString("Regions affected by the tsunami is the active region (region %1) and")
                                  .arg(this->activeRegion->getRegion()));
    this->boardModel->sendMessage(QString("regions bordering on the active region and the sea."));
    this->boardModel->sendMessage(QString("The affected region(s): %1.")
                                  .arg(Common::listUpRegions(this->affectedRegions)));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
    return;
}

Instruction *TsunamiInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step = 1;
        this->tsunamiDamage = this->boardModel->drawCard()->getShapeNumberSum(this->firstEvent->getShapeNumberAmounts());
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage(QString("The damage in each affected region caused by the tsunami is %1.")
                                      .arg(this->tsunamiDamage));

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::CIVIL_SERVICE))
        {
            this->boardModel->sendMessage("Advance (CIVIL SERVICE):");
            this->damagePerTribe = 3;
            this->damagePerCityAV = 5;
        }

        this->boardModel->sendMessage(QString("For every %1 damage, one tribe will decimated.")
                                      .arg(this->damagePerTribe));
        this->boardModel->sendMessage(QString("With no tribes left, for every %1 damage, one City AV will be reduced.")
                                      .arg(this->cityDamage));
        this->boardModel->sendMessage(QString("With the city destroyed, for every %1 damage, one Wonder will be decimated.")
                                      .arg(this->wonderDamage));
        this->boardModel->sendMessage(" ");

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
                if(currentDamage < this->cityDamage)
                {
                    currentDamage = 0;
                }
                else
                {
                    if(affectedRegion->getCityAV() > 0)
                    {
                        affectedRegion->decreaseCityAV(1);
                        totalCityDamage++;
                        currentDamage -= this->cityDamage;
                    }
                    affectedRegion->decimateZeroAVCity();
                }
            }
            else
            {
                currentDamage = 0;
            }
        }

        this->boardModel->sendMessage(QString("In region %1, the tsunami results in the following losses.")
                                      .arg(affectedRegion->getRegion()));
        this->boardModel->sendMessage(QString("Tribes: %1")
                                      .arg(totalTribeDamage));
        this->boardModel->sendMessage(QString("CityAV: %1")
                                      .arg(totalCityDamage));

        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage(QString("Affected regions left: %1.")
                                      .arg(Common::listUpRegions(this->affectedRegions)));
        this->boardModel->sendMessage("Press Done to continue.");
        this->boardModel->sendMessage(" ");
        return this;
    }

    this->boardModel->unsetActiveRegion();
    this->boardModel->sendMessage("The Tsunami has subsided and the event ends.");
    this->boardModel->sendMessage(" ");
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
