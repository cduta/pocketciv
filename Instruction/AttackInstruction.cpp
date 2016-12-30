#include "AttackInstruction.hpp"

#include "Common.hpp"

AttackInstruction::AttackInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what, int attackingForce)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), what(what), attackingForce(attackingForce)
{
    this->nextInstruction->setKeepInstruction(true);
    this->tribeAttackReduce = 1;
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

    this->boardModel->printMessage("The attacking force will pillage through the empire.");
    this->boardModel->printMessage(" ");

    this->boardModel->printMessage(QString("Reduce the attacking force by 1."));
    this->boardModel->printMessage(QString("As long as at least 1 attacking force is reduced, reduce 1 Tribe."));
    this->boardModel->printMessage(QString("Repeat until no more tribes are left."));
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::METAL_WORKING))
    {
        this->boardModel->printMessage(QString("Advance (METAL WORKING):"));
        this->boardModel->printMessage(QString("Reduce one additional attacking force in this step."));
        this->boardModel->printMessage(" ");
        this->tribeAttackReduce++;
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MILITARY))
    {
        this->boardModel->printMessage(QString("Advance (MILITARY):"));
        this->boardModel->printMessage(QString("Reduce one additional attacking force in this step."));
        this->boardModel->printMessage(" ");
        this->tribeAttackReduce++;
    }

    this->boardModel->printMessage(QString("After no more tribes are left, reduce the attacking force by 5."));
    this->boardModel->printMessage(QString("As long as at least 1 attacking force is reduced, reduce 1 City AV"));
    this->boardModel->printMessage(QString("and remove 2 gold."));
    this->boardModel->printMessage(QString("Repeat until no more City AV is left."));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage(QString("If at any point the attacking force becomes 0, the event ends."));
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::ARCHITECTURE))
    {
        this->cityAVAttackReduce += 3;
        this->boardModel->printMessage("Advance (ARCHITECTURE):");
        this->boardModel->printMessage("Reduce 3 additional attacking force in this step.");
        this->boardModel->printMessage(" ");
    }

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
        this->attackingForce -= this->tribeAttackReduce;

        newTribes--;
    }

    while(newCityAV > 0 && this->attackingForce > 0)
    {
        this->attackingForce-= this->cityAVAttackReduce;

        newCityAV--;
        goldLost += 2;
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
