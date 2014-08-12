#include "RegionModel.hpp"

#include <boost/assert.hpp>

RegionModel::RegionModel(int region, QObject *parent)
    : QObject(parent),
      region(region),
      tribes(0),
      movedTribes(0),
      selectedTribes(0),
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

void RegionModel::removeFromMovedTribes(int movedTribes)
{
    this->movedTribes -= movedTribes;
    return;
}

void RegionModel::mergeMovedTribes()
{
    this->tribes += this->movedTribes;
    this->movedTribes = 0;
    return;
}

void RegionModel::decimateSelectedTribes()
{
    this->setTribes(this->tribes - this->selectedTribes);
    this->setSelectedTribes(0);
    return;
}

void RegionModel::addCityAV(int cityAV)
{
    this->cityAV += cityAV;

    if(this->cityAV < 0)
    {
        this->cityAV = 0;
    }

    return;
}

void RegionModel::decreaseCityAV(int cityAV)
{
    this->cityAV -= cityAV;

    if(this->cityAV < 0)
    {
        this->cityAV = 0;
    }

    return;
}

void RegionModel::decimateZeroAVCity()
{
    if(this->cityAV <= 0)
    {
        this->cityAV = 0;
        this->city = false;
    }
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

int RegionModel::getSelectedTribes() const
{
    return this->selectedTribes;
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

void RegionModel::setCity(bool city)
{
    this->city = city;
    return;
}

void RegionModel::setCityAV(int cityAV)
{
    this->cityAV = cityAV;
    return;
}

void RegionModel::setSelectedTribes(int selectedTribes)
{
    assert(selectedTribes <= this->tribes);
    this->selectedTribes = selectedTribes;
    return;
}

void RegionModel::setMountain(bool mountain)
{
    this->mountain = mountain;
    return;
}

void RegionModel::setForest(bool forest)
{
    this->forest = forest;
    return;
}

void RegionModel::setFarm(bool farm)
{
    this->farm = farm;
    return;
}

void RegionModel::setDesert(bool desert)
{
    this->desert = desert;
    return;
}

void RegionModel::setSelected(bool selected)
{
    this->selected = selected;
}
