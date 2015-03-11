#include "AttackInstruction.hpp"

#include "Common.hpp"

AttackInstruction::AttackInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what, int attackingForce)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), what(what), attackingForce(attackingForce)
{
    this->nextInstruction->setKeepInstruction(true);
    this->cityAVAttackReduce = 5;
}

void AttackInstruction::initInstruction()
{
    this->boardModel->sendMessage(QString("%1 ATTACK:").arg(this->what));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage(QString("The attacking force is %1 in region %2.")
                                  .arg(this->attackingForce)
                                  .arg(this->boardModel->refActiveRegion()->getRegion()));

    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("The attacking force will pillage through the empire.");
    this->boardModel->sendMessage("In each region they visit, City AV, tribes and gold is reduced.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("For each attacking force a tribe is reduced until no more tribes are left.");

    if(this->boardModel->getAdvancesAquired().contains(AdvanceModel::ARCHITECTURE))
    {
        this->cityAVAttackReduce = 8;
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Advance (ARCHITECTURE):");
        this->boardModel->sendMessage("Then, for each 8 attacking force a City AV and 2 gold are reduced until City AV is 0.");
        this->boardModel->sendMessage(" ");
    }
    else
    {
        this->cityAVAttackReduce = 5;
        this->boardModel->sendMessage("Then, for each 5 attacking force a City AV and 2 gold are reduced until City AV is 0.");
        this->boardModel->sendMessage(" ");
    }

    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
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

    this->boardModel->sendMessage(QString("Reduced %1 of %2 tribes and %3 of %4 city AV in region %5 as well as %6 gold.")
                                  .arg(tribes-newTribes)
                                  .arg(tribes)
                                  .arg(cityAV-newCityAV)
                                  .arg(cityAV)
                                  .arg(this->boardModel->refActiveRegion()->getRegion())
                                  .arg(goldLost));
    this->boardModel->sendMessage(QString("There is %1 attacking force left.")
                                  .arg(this->attackingForce));

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
            this->boardModel->sendMessage(QString("The next attacked region is %1.").arg(current.first()->getRegion()));
            this->boardModel->sendMessage("Press Done to continue.");
            this->boardModel->sendMessage(" ");
            return this;
        }
        else if(current.size() > 1)
        {
            this->boardModel->sendMessage("There are multiple regions possible to move the attacking forces into.");

            this->boardModel->sendMessage(QString("Those regions are %1.")
                                          .arg(Common::listUpRegions(current)));

            this->possibleRegions = current.toSet();
            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage("Choose one and press Done to continue.");
            this->boardModel->sendMessage(" ");
            return this;
        }

        this->boardModel->sendMessage("There is no region left for the attacking force to move into.");
    }
    else
    {
        this->boardModel->sendMessage("There are no more attacking forces left.");
    }

    this->boardModel->sendMessage(QString("The %1 ATTACK has ended.").arg(this->what));
    this->boardModel->sendMessage(" ");

    this->boardModel->unsetActiveRegion();
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
