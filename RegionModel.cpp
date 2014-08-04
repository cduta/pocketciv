#include "RegionModel.hpp"

RegionModel::RegionModel(QObject *parent)
    : QObject(parent),
      tribes(0),
      mountain(false),
      forest(false),
      desert(false)
{}

void RegionModel::toggleMountain()
{
    this->mountain = !this->mountain;
    return;
}

void RegionModel::toggleForest()
{
    this->forest = !this->forest;
    return;
}

int RegionModel::getTribes() const
{
    return this->tribes;
}

bool RegionModel::hasMountain() const
{
    return this->mountain;
}

bool RegionModel::hasForest() const
{
    return this->forest;
}

bool RegionModel::hasDesert() const
{
    return this->desert;
}

void RegionModel::setTribes(int tribes)
{
    this->tribes = tribes;
    return;
}

void RegionModel::setDesert()
{
    this->desert = true;
    return;
}

void RegionModel::unsetDesert()
{
    this->desert = false;
    return;
}
