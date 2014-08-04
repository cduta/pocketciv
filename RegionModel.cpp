#include "RegionModel.hpp"

RegionModel::RegionModel(int region, QObject *parent)
    : QObject(parent),
      region(region),
      tribes(0),
      movedTribes(0),
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

void RegionModel::addToMovedTribes(int movedTribes)
{
    this->movedTribes += movedTribes;
    return;
}

void RegionModel::mergeMovedTribes()
{
    this->tribes += this->movedTribes;
    this->movedTribes = 0;
    return;
}

int RegionModel::getRegion() const
{
    return this->region;
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

void RegionModel::unsetMountain()
{
    this->mountain = false;
    return;
}

void RegionModel::unsetForest()
{
    this->forest = false;
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
