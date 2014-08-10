#include "RegionModel.hpp"

RegionModel::RegionModel(int region, QObject *parent)
    : QObject(parent),
      region(region),
      tribes(0),
      movedTribes(0),
      mountain(false),
      volcano(false),
      forest(false),
      farm(false),
      desert(false),
      selected(false),
      city(false),
      cityAV(0)
{}

int RegionModel::getTribeSupportCount() const
{
    int result = 0;

    if(this->mountain)
    {
        result++;
    }

    if(this->volcano)
    {
        result++;
    }

    if(this->forest)
    {
        result++;
    }

    if(this->farm)
    {
        result++;
    }

    result += this->cityAV;

    return result;
}

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

int RegionModel::getMovedTribes() const
{
    return this->movedTribes;
}

bool RegionModel::hasMountain() const
{
    return this->mountain;
}

bool RegionModel::hasVolcano() const
{
    return this->volcano;
}

bool RegionModel::hasForest() const
{
    return this->forest;
}

bool RegionModel::hasFarm() const
{
    return this->farm;
}

bool RegionModel::hasDesert() const
{
    return this->desert;
}

bool RegionModel::isSelected() const
{
    return this->selected;
}

bool RegionModel::hasCity() const
{
    return this->city;
}

int RegionModel::getCityAV() const
{
    return this->cityAV;
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

void RegionModel::setSelected(bool selected)
{
    this->selected = selected;
}
