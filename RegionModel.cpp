#include "RegionModel.hpp"

#include <boost/assert.hpp>

#include <QDataStream>

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
      faultLine(false),
      selected(false),
      city(false),
      capitol(false),
      cityAV(0),
      selectedCityAV(0),
      advanceAquired(false)
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

void RegionModel::decimateTribes(int tribes)
{
    if(tribes < 0)
    {
        return;
    }

    this->tribes -= tribes;

    if(this->tribes < 0)
    {
        this->tribes = 0;
    }

    return;
}

void RegionModel::decimateSelectedTribes()
{
    this->setTribes(this->tribes - this->selectedTribes);
    this->setSelectedTribes(0);
    return;
}

void RegionModel::increaseCityAV(int cityAV)
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
        this->city = false;

        if(this->wonders.contains(WonderModel::CITY_OF_ATLANTIS) && this->wonders[WonderModel::CITY_OF_ATLANTIS] > 0)
        {
            this->wonders.remove(WonderModel::CITY_OF_ATLANTIS);
        }
    }
    return;
}

void RegionModel::reduceSelectedCityAV()
{
    this->cityAV -= this->selectedCityAV;
    this->selectedCityAV = 0;
    return;
}

void RegionModel::buildWonder(WonderModel::Wonder wonder)
{
    if(this->wonders.contains(wonder))
    {
        this->wonders[wonder] += 1;
    }
    else
    {
        this->wonders.insert(wonder, 1);
    }
    return;
}

void RegionModel::decimateWonder(WonderModel::Wonder wonder)
{
    if(this->wonders.contains(wonder))
    {
        if(this->wonders[wonder] > 1)
        {
            this->wonders[wonder] -= 1;
        }
        else
        {
            this->wonders.remove(wonder);
        }
    }
    return;
}

void RegionModel::decimateAllWonders()
{
    this->wonders.clear();
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

bool RegionModel::hasTribes() const
{
    return this->tribes > 0;
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

bool RegionModel::hasFaultLine() const
{
    return this->faultLine;
}

bool RegionModel::isSelected() const
{
    return this->selected;
}

bool RegionModel::hasCity() const
{
    return this->city;
}

bool RegionModel::isCapitolRegion() const
{
    return this->capitol;
}

int RegionModel::getCityAV() const
{
    return this->cityAV;
}

int RegionModel::getSelectedCityAV() const
{
    return this->selectedCityAV;
}

bool RegionModel::hasAdvanceAquired() const
{
    return this->advanceAquired;
}

bool RegionModel::hasWonders() const
{
    foreach(int wonderCount, this->wonders.values())
    {
        if(wonderCount > 0)
        {
            return true;
        }
    }

    return false;
}

bool RegionModel::hasWonder(WonderModel::Wonder wonder) const
{
    return this->wonders.contains(wonder) && this->wonders[wonder] > 0;
}

QMap<WonderModel::Wonder, int> RegionModel::getBuiltWonders() const
{
    return this->wonders;
}

void RegionModel::setTribes(int tribes)
{
    this->tribes = tribes;

    if(this->tribes < 0)
    {
        this->tribes = 0;
    }
    return;
}

void RegionModel::setCity(bool city)
{
    this->city = city;
    if(!this->city)
    {
        this->setCityAV(0);
        if(this->wonders.contains(WonderModel::CITY_OF_ATLANTIS) && this->wonders[WonderModel::CITY_OF_ATLANTIS] > 0)
        {
            this->wonders.remove(WonderModel::CITY_OF_ATLANTIS);
        }
    }

    return;
}

void RegionModel::setCapitolRegion(bool capitol)
{
    assert(this->hasCity());

    this->capitol = capitol;
    return;
}

void RegionModel::toggleCapitol()
{
    this->setCapitolRegion(!this->isCapitolRegion());
    return;
}

void RegionModel::setCityAV(int cityAV)
{
    this->cityAV = cityAV;

    if(this->cityAV < 0)
    {
        this->cityAV = 0;
    }
    return;
}

void RegionModel::setSelectedTribes(int selectedTribes)
{
    assert(selectedTribes <= this->tribes);
    this->selectedTribes = selectedTribes;
    return;
}

void RegionModel::setSelectedCityAV(int selectedCityAV)
{
    assert(selectedCityAV <= this->cityAV);
    this->selectedCityAV = selectedCityAV;
    return;
}

void RegionModel::setMountain(bool mountain)
{
    this->mountain = mountain;
    if(this->mountain)
    {
        this->volcano = false;
    }
    return;
}

void RegionModel::setVolcano(bool volcano)
{
    this->volcano = volcano;
    if(this->volcano)
    {
        this->mountain = false;
    }
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

void RegionModel::setFaultLine(bool faultLine)
{
    this->faultLine = faultLine;
    return;
}

void RegionModel::setSelected(bool selected)
{
    this->selected = selected;
}

void RegionModel::setAdvanceAquired(bool advanceAquired)
{
    this->advanceAquired = advanceAquired;
}

void RegionModel::serialize(QDataStream &writer) const
{
    writer << this->region;
    writer << this->tribes;
    writer << this->movedTribes;
    writer << this->selectedTribes;
    writer << this->mountain;
    writer << this->volcano;
    writer << this->forest;
    writer << this->farm;
    writer << this->desert;
    writer << this->faultLine;
    writer << this->selected;
    writer << this->city;
    writer << this->capitol;
    writer << this->cityAV;
    writer << this->selectedCityAV;
    writer << this->advanceAquired;

    writer << this->wonders.keys().count();
    foreach(WonderModel::Wonder wonder, this->wonders.keys())
    {
        writer << (int) wonder;
        writer << this->wonders[wonder];
    }

    return;
}

void RegionModel::deserialize(QDataStream &reader)
{
    reader >> this->region;
    reader >> this->tribes;
    reader >> this->movedTribes;
    reader >> this->selectedTribes;
    reader >> this->mountain;
    reader >> this->volcano;
    reader >> this->forest;
    reader >> this->farm;
    reader >> this->desert;
    reader >> this->faultLine;
    reader >> this->selected;
    reader >> this->city;
    reader >> this->capitol;
    reader >> this->cityAV;
    reader >> this->selectedCityAV;
    reader >> this->advanceAquired;

    int wonder;
    int wonderCount;
    int wonderEntryCount;

    reader >> wonderEntryCount;
    this->wonders.clear();
    for(int i = 0; i < wonderEntryCount; ++i)
    {
        reader >> wonder;
        reader >> wonderCount;
        this->wonders.insert(WonderModel::Wonder(wonder), wonderCount);
    }

    return;
}
