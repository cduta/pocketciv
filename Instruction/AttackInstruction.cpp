#include "AttackInstruction.hpp"

#include "Common.hpp"

AttackInstruction::AttackInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what, int attackingForce)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), what(what), attackingForce(attackingForce)
{
    this->nextInstruction->setKeepInstruction(true);
    this->cityAVAttackReduce = 5;
    this->firstAttack = true;
}

void AttackInstruction::initInstruction()
{
    this->boardModel->printMessage(QString("%1 ATTACK:").arg(this->what));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage(QString("The attacking force is %1 in region %2.")
                                  .arg(this->attackingForce)
                                  .arg(this->boardModel->refActiveRegion()->getRegion()));

    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("The attacking force will pillage through the empire.");
    this->boardModel->printMessage("In each region they visit, City AV, tribes and gold is reduced.");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("For each attacking force a tribe is reduced until no more tribes are left.");

    if(this->boardModel->getAdvancesAquired().contains(AdvanceModel::ARCHITECTURE))
    {
        this->cityAVAttackReduce = 8;
        this->boardModel->printMessage(" ");
        this->boardModel->printMessage("Advance (ARCHITECTURE):");
        this->boardModel->printMessage("Then, for each 8 attacking force a City AV and 2 gold are reduced until City AV is 0.");
        this->boardModel->printMessage(" ");
    }
    else
    {
        this->cityAVAttackReduce = 5;
        this->boardModel->printMessage("Then, for each 5 attacking force a City AV and 2 gold are reduced until City AV is 0.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *AttackInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(this->possibleRegions.size() > 1 && this->possibleRegions.contains(regionModel))
        {
            this->boardModel->setActiveRegion(regionModel->getRegion());
            return this;
        }
    }

    return this;
}

Instruction *AttackInstruction::triggerDone()
{
    int tribes = this->boardModel->refActiveRegion()->getTribes();
    int newTribes = tribes;
    int cityAV = this->boardModel->refActiveRegion()->getCityAV();
    int newCityAV = cityAV;
    int goldLost = 0;

    if(!this->firstAttack &&
       tribes > 0 &&
       this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN))
    {
        this->attackingForce -= 2;
        this->boardModel->printMessage("Advance (EQUESTRIAN):");
        this->boardModel->printMessage("Because the attacking forces move into a region with tribes,");
        this->boardModel->printMessage("the attacking force is reduced by 2.");
        this->boardModel->printMessage(" ");
    }

    while(newTribes > 0 && this->attackingForce > 0)
    {
        this->attackingForce--;
        newTribes--;
    }

    while(newCityAV > 0 && this->attackingForce > 0)
    {

        this->attackingForce-= this->cityAVAttackReduce;

        if(this->attackingForce >= 0)
        {
            newCityAV--;
            goldLost += 2;
        }
        else
        {
            this->attackingForce = 0;
        }
    }

    if(this->attackingForce < 0)
    {
        this->attackingForce = 0;
    }

    this->boardModel->refActiveRegion()->setTribes(newTribes);
    this->boardModel->refActiveRegion()->setCityAV(newCityAV);
    this->boardModel->refActiveRegion()->decimateZeroAVCity();
    this->boardModel->removeGold(goldLost);

    this->boardModel->printMessage(QString("Reduced %1 of %2 tribes and %3 of %4 city AV in region %5 as well as %6 gold.")
                                  .arg(tribes-newTribes)
                                  .arg(tribes)
                                  .arg(cityAV-newCityAV)
                                  .arg(cityAV)
                                  .arg(this->boardModel->refActiveRegion()->getRegion())
                                  .arg(goldLost));
    this->boardModel->printMessage(QString("There is %1 attacking force left.")
                                  .arg(this->attackingForce));

    this->firstAttack = false;

    if(this->attackingForce > 0)
    {
        QMap<int, RegionModel *> adjacent = this->boardModel->getAdjacentRegions(this->boardModel->refActiveRegion()->getRegion());
        QList<RegionModel *> current;

        foreach(RegionModel *regionModel, adjacent.values())
        {
            if(regionModel->getTribes() > 0)
            {
                if(current.isEmpty() ||
                   regionModel->getTribes() < current.first()->getTribes() ||
                  (regionModel->getTribes() == current.first()->getTribes() && regionModel->getCityAV() > current.first()->getCityAV()))
                {
                    current.clear();
                    current.append(regionModel);
                }
                else if(regionModel->getTribes() == current.first()->getTribes() &&
                        regionModel->getCityAV() == current.first()->getCityAV())
                {
                    current.append(regionModel);
                }
            }
        }

        this->possibleRegions.clear();
        if(current.size() == 1)
        {
            this->boardModel->setActiveRegion(current.first()->getRegion());
            this->boardModel->printMessage(QString("The next attacked region is %1.").arg(current.first()->getRegion()));
            this->boardModel->printMessage("Press Done to continue.");
            this->boardModel->printMessage(" ");
            return this;
        }
        else if(current.size() > 1)
        {
            this->boardModel->printMessage("There are multiple regions possible to move the attacking forces into.");

            this->boardModel->printMessage(QString("Those regions are %1.")
                                          .arg(Common::listUpRegions(current)));

            this->possibleRegions = current.toSet();
            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("Choose one and press Done to continue.");
            this->boardModel->printMessage(" ");
            return this;
        }
        else
        {
            this->boardModel->printMessage("There is no region left for the attacking force to move into.");
        }
    }
    else
    {
        this->boardModel->printMessage("There are no more attacking forces left.");
    }

    this->boardModel->printMessage(QString("The %1 ATTACK has ended.").arg(this->what));
    this->boardModel->printMessage(" ");

    this->boardModel->unsetActiveRegion();
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
