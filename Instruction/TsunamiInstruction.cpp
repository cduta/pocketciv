#include "TsunamiInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

TsunamiInstruction::TsunamiInstruction(BoardModel *boardModel, RegionModel *activeRegion, Instruction *nextInstruction, const Event *firstEvent)
    : Instruction(), firstEvent(firstEvent)
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
        this->boardModel->sendMessage(QString("The damage in each region caused by the tsunami is %1.")
                                      .arg(this->tsunamiDamage));
        this->boardModel->sendMessage(" ");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(!this->affectedRegions.isEmpty())
    {
        RegionModel *affectedRegion = this->affectedRegions.takeFirst();
        int currentDamage = this->tsunamiDamage;
        int tribeDamage = 0;
        int cityDamage = 0;
        //TODO: Add Wonder decimation for 3 damage.

        while(currentDamage > 0)
        {
            if(affectedRegion->hasTribes())
            {
                affectedRegion->decimateTribes(1);
                tribeDamage++;
                currentDamage -= 1;
            }
            else if(affectedRegion->hasCity())
            {
                if(currentDamage < 2)
                {
                    currentDamage = 0;
                }
                else
                {
                    if(affectedRegion->getCityAV() > 0)
                    {
                        affectedRegion->decreaseCityAV(1);
                        cityDamage++;
                        currentDamage -= 2;
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
                                      .arg(tribeDamage));
        this->boardModel->sendMessage(QString("CityAV: %1")
                                      .arg(cityDamage));

        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage(QString("Affected regions left: %1.")
                                      .arg(Common::listUpRegions(this->affectedRegions)));
        this->boardModel->sendMessage("Press Done to continue.");
        this->boardModel->sendMessage(" ");
        return this;
    }

    this->boardModel->sendMessage("The Tsunami has subsided and the event ends...");
    this->boardModel->sendMessage(" ");
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
