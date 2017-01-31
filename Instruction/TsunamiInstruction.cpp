#include "TsunamiInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"
#include "WonderDialog.hpp"

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

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MEDICINE))
    {
        this->boardModel->printMessage("Advance (MEDICINE):");
        this->boardModel->printMessage("After the tsunami, every affected region has 2 tribes added to it.");
        this->boardModel->printMessage(" ");
    }

    this->possibleAffectedRegions.append(this->activeRegion);

    QMap<int, RegionModel *> regions = this->boardModel->getAdjacentRegions(this->activeRegion->getRegion());

    foreach(int region, regions.keys())
    {
        RegionModel *regionModel = regions.value(region);
        if(this->boardModel->bordersOnSea(regionModel->getRegion()))
        {
            this->possibleAffectedRegions.append(regionModel);
        }
    }

    this->boardModel->printMessage(QString("Regions affected by the tsunami is region %1 and")
                                  .arg(this->activeRegion->getRegion()));
    this->boardModel->printMessage(QString("regions bordering on it and the sea."));

    QString regionString = "region";

    if(this->possibleAffectedRegions.count() == 1)
    {
        regionString = "regions";
    }

    this->boardModel->printMessage(QString("The %1 hit by the tsunami: %2.")
                                  .arg(regionString)
                                  .arg(Common::listUpRegions(this->possibleAffectedRegions)));
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
        this->boardModel->printMessage(QString("The damage in the regions caused by the tsunami is %1.")
                                      .arg(this->tsunamiDamage));


        this->boardModel->printMessage(QString("For every 1 damage, one tribe will decimated."));
        this->boardModel->printMessage(QString("With no tribes left, for every 2 damage, one City AV will be reduced."));
        this->boardModel->printMessage(QString("With no tribes and the city destroyed, for every 3 damage,"));
        this->boardModel->printMessage("one Wonder of your choice will be decimated.");
        this->boardModel->printMessage(" ");

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::CIVIL_SERVICE))
        {
            this->boardModel->printMessage(QString("Advance (CIVIL SERVICE)"));
            this->boardModel->printMessage(QString("Because of CIVIL SERVICE, for every 3 damage, one tribe will be decimated and"));
            this->boardModel->printMessage(QString("for every 5 damage, one City AV will be reduced."));
            this->boardModel->printMessage(QString(" "));

            this->damagePerTribe = 3;
            this->damagePerCityAV = 5;
        }

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING))
        {
            this->boardModel->printMessage(QString("Advance (ENGINEERING)"));
            this->boardModel->printMessage(QString("Because of ENGINEERING, for every 10 damage, one wonder of your choice will be decimated."));
            this->boardModel->printMessage(QString(" "));
            this->damagePerWonder = 10;
        }

        POCKET_CIV_END_OF_ERA_CHECK
    }

    while(!this->possibleAffectedRegions.isEmpty())
    {
        RegionModel *possibleAffectedRegion = this->possibleAffectedRegions.takeFirst();
        int currentDamage = this->tsunamiDamage;
        int totalTribeDamage = 0;
        int totalCityDamage = 0;
        int totalWonderDamage = 0;

        while(currentDamage > 0)
        {
            if(possibleAffectedRegion->hasTribes())
            {
                if(currentDamage < this->damagePerTribe)
                {
                    currentDamage = 0;
                }
                else
                {
                    possibleAffectedRegion->decimateTribes(1);
                    totalTribeDamage++;
                    currentDamage -= this->damagePerTribe;
                }
            }
            else if(possibleAffectedRegion->hasCity())
            {
                if(currentDamage < this->damagePerCityAV)
                {
                    currentDamage = 0;
                }
                else
                {
                    if(possibleAffectedRegion->getCityAV() > 0)
                    {
                        possibleAffectedRegion->decreaseCityAV(1);
                        totalCityDamage++;
                        currentDamage -= this->damagePerCityAV;
                    }
                    possibleAffectedRegion->decimateZeroAVCity();
                }
            }
            else if(possibleAffectedRegion->hasWonders())
            {
                if(currentDamage < this->damagePerWonder)
                {
                    currentDamage = 0;
                }
                else
                {
                    totalWonderDamage++;
                    currentDamage -= this->damagePerWonder;
                }
            }
            else
            {
                currentDamage = 0;
            }
        }


        if(totalTribeDamage + totalCityDamage + totalWonderDamage > 0)
        {
            QMap<WonderModel::Wonder, int> wonderDamage;
            while(totalWonderDamage > 0)
            {
                WonderDialog *wonderDialog = new WonderDialog(this->boardModel, WonderDescription::SELECTION, possibleAffectedRegion->getRegion());
                wonderDialog->setSelectionTotal(totalWonderDamage);
                wonderDialog->exec();
                wonderDamage = wonderDialog->getSelectedWonders();
                wonderDialog->deleteLater();
            }

            this->boardModel->printMessage(QString("In region %1, the tsunami results in the following losses.")
                                          .arg(possibleAffectedRegion->getRegion()));
            this->boardModel->printMessage(QString("Tribes decimated: %1")
                                          .arg(totalTribeDamage));
            this->boardModel->printMessage(QString("CityAV reduced: %1")
                                          .arg(totalCityDamage));
            this->boardModel->printMessage(" ");

            this->boardModel->printMessage(QString("Wonders decimated: %1").arg(WonderModel::listUpWonders(wonderDamage)));
            this->affectedRegions.insert(possibleAffectedRegion->getRegion(), possibleAffectedRegion);
        }
        else
        {
            this->boardModel->printMessage(QString("In region %1, the tsunami had no effect.")
                                          .arg(possibleAffectedRegion->getRegion()));
            this->boardModel->printMessage(" ");
        }
    }

    this->boardModel->unsetActiveRegion();

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MEDICINE))
    {
        this->boardModel->printMessage("Advance (MEDICINE):");
        this->boardModel->printMessage(QString("Two tribes are added to every affected region (%1).")
                                       .arg(Common::listUpRegions(this->affectedRegions.values())));
        this->boardModel->printMessage(" ");

        foreach(RegionModel *regionModel, this->affectedRegions.values())
        {
            regionModel->setTribes(regionModel->getTribes() + 2);
        }
    }

    this->boardModel->printMessage("The Tsunami has subsided and the event ends.");
    this->boardModel->printMessage(" ");
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
