#include "BoardModel.hpp"

#include <QMap>
#include <QDataStream>

#include "Common.hpp"
#include "Event/AnarchyEvent.hpp"
#include "Event/BanditsEvent.hpp"
#include "Event/CivilWarEvent.hpp"
#include "Event/CorruptionEvent.hpp"
#include "Event/EarthquakeEvent.hpp"
#include "Event/EpidemicEvent.hpp"
#include "Event/FamineEvent.hpp"
#include "Event/FloodEvent.hpp"
#include "Event/SandstormEvent.hpp"
#include "Event/SuperstitionEvent.hpp"
#include "Event/TribalWarEvent.hpp"
#include "Event/UprisingEvent.hpp"
#include "Event/VolcanoEvent.hpp"
#include "Event/VisitationEvent.hpp"

BoardModel::BoardModel(int width, int height, QObject *parent)
    : QObject(parent),
      activeRegion(NULL),
      buildCity(false),
      buildFarm(false),
      expedition(false),
      aquireAdvances(false),
      buildWonder(false),
      collectTaxes(false),
      forestation(false),
      mining(false),
      doneEnabled(true),
      gold(0),
      era(1),
      lastEra(8),
      gloryScore(0),
      originalCard(NULL)
{
    this->newBoard(width, height);
    this->initializeCards();
}

BoardModel::~BoardModel()
{
    this->clearBoard();
}

bool BoardModel::toggleHexToRegion(int region, int x, int y)
{
    HexModel *hex = this->hexModels[x][y];
    QSet<HexModel *> regionSet;

    if(this->regionHexes.contains(region))
    {
        regionSet = this->regionHexes[region];
    }

    if(hex->getRegion() == region)
    {
        regionSet.remove(hex);
    }
    else
    {
        regionSet.insert(hex);
    }

    if(!this->checkRegionHexSet(regionSet))
    {
        return false;
    }

    if(hex->getRegion() == region)
    {
        hex->unsetRegion();
    }
    else
    {
        hex->setRegion(region);
    }

    this->regionHexes.insert(region, regionSet);

    emit this->boardUpdated();
    return true;
}

void BoardModel::newBoard(int width, int height)
{
    this->clearBoard();

    for(int x = 0; x < width; ++x)
    {
        this->hexModels.append(QList<HexModel *>());
        for(int y = 0; y < height; ++y)
        {
            this->hexModels[x].append(new HexModel(x,y, true, DRAW_NO_BORDER, this));
        }
    }

    this->initializeBoard();

    emit this->boardUpdated();
    return;
}

void BoardModel::initializeBoard()
{
    for(int x = 0; x < this->hexModels.size(); ++x)
    {
        for(int y = 0; y < this->hexModels[x].size(); ++y)
        {
            QMap<int, HexModel *> adjacendHexes;

            if(x%2 != 0)
            {
                if(x-1 >= 0 && y-1 >= 0)
                {
                    adjacendHexes.insert(DRAW_UPPER_LEFT_BORDER, this->hexModels[x-1][y-1]);
                }

                if(y-1 >= 0)
                {
                    adjacendHexes.insert(DRAW_UPPER_CENTER_BORDER, this->hexModels[x][y-1]);
                }

                if(x+1 < this->hexModels.size() && y-1 >= 0)
                {
                    adjacendHexes.insert(DRAW_UPPER_RIGHT_BORDER, this->hexModels[x+1][y-1]);
                }

                if(x-1 >= 0)
                {
                    adjacendHexes.insert(DRAW_LOWER_LEFT_BORDER, this->hexModels[x-1][y]);
                }

                if(y+1 < this->hexModels[x].size())
                {
                    adjacendHexes.insert(DRAW_LOWER_CENTER_BORDER, this->hexModels[x][y+1]);
                }

                if(x+1 < this->hexModels.size())
                {
                    adjacendHexes.insert(DRAW_LOWER_RIGHT_BORDER, this->hexModels[x+1][y]);
                }
            }
            else
            {
                if(x-1 >= 0)
                {
                    adjacendHexes.insert(DRAW_UPPER_LEFT_BORDER, this->hexModels[x-1][y]);
                }

                if(y-1 >= 0)
                {
                    adjacendHexes.insert(DRAW_UPPER_CENTER_BORDER, this->hexModels[x][y-1]);
                }

                if(x+1 < this->hexModels.size())
                {
                    adjacendHexes.insert(DRAW_UPPER_RIGHT_BORDER, this->hexModels[x+1][y]);
                }

                if(x-1 >= 0 && y+1 < this->hexModels[x].size())
                {
                    adjacendHexes.insert(DRAW_LOWER_LEFT_BORDER, this->hexModels[x-1][y+1]);
                }

                if(y+1 < this->hexModels[x].size())
                {
                    adjacendHexes.insert(DRAW_LOWER_CENTER_BORDER, this->hexModels[x][y+1]);
                }

                if(x+1 < this->hexModels.size() && y+1 < this->hexModels[x].size())
                {
                    adjacendHexes.insert(DRAW_LOWER_RIGHT_BORDER, this->hexModels[x+1][y+1]);
                }
            }

            this->hexModels[x][y]->setAdjacentHexes(adjacendHexes);
        }
    }

    this->enableRegionSelectableHexes();

    return;
}

void BoardModel::enableRegionSelectableHexes()
{
    for(int x = 0; x < this->hexModels.size(); ++x)
    {
        if(!this->hexModels[x].isEmpty())
        {
            this->hexModels[x][0]->setEnable(false);
            this->hexModels[x][this->hexModels[x].size()-1]->setEnable(false);
        }
    }

    if(!this->hexModels.isEmpty())
    {
        for(int y = 1; y < this->hexModels[0].size()-1; ++y)
        {
            this->hexModels[0][y]->setEnable(false);
        }

        for(int y = 1; y < this->hexModels[this->hexModels.size()-1].size()-1; ++y)
        {
            this->hexModels[this->hexModels.size()-1][y]->setEnable(false);
        }
    }

    return;
}

void BoardModel::groupSeas()
{
    foreach(QList<HexModel *> hexes, this->hexModels)
    {
        foreach(HexModel *hex, hexes)
        {
            if(!hex->isSea())
            {
                continue;
            }

            bool grouped = false;
            foreach(QSet<HexModel *> sea, this->seas)
            {
                grouped = grouped || sea.contains(hex);
            }

            if(!grouped)
            {
                QSet<HexModel *> seaGroup;
                seaGroup.insert(hex);
                QSet<HexModel *> next = hex->getAdjacentSeaHexes();

                while(!next.isEmpty())
                {
                    HexModel *current = next.toList().first();
                    next.remove(current);
                    next = next.unite(current->getAdjacentSeaHexes().subtract(seaGroup).subtract(next));
                    seaGroup.insert(current);
                }

                this->seas.append(seaGroup);
            }
        }
    }

    return;
}

void BoardModel::initialRegionModels()
{
    foreach(int region, this->regionHexes.keys())
    {
        QList<HexModel *> regionHexes;

        foreach(HexModel *hex, this->regionHexes[region].toList())
        {
            if(!hex->showRegionNumber())
            {
                regionHexes.append(hex);
            }
        }

        HexModel *hex = regionHexes[Common::randomSeed()%regionHexes.size()];
        RegionModel *regionModel = new RegionModel(region, hex);
        hex->setRepresentativeHex(true, regionModel);
        this->regions.insert(region, regionModel);
    }

    return;
}

bool BoardModel::canMoveTribes(int fromRegion, int toRegion)
{
    // Is Adjacent region?
    QMap<int, RegionModel *> adjacentRegions = this->getAdjacentRegions(fromRegion);

    foreach(int adjacentRegion, adjacentRegions.keys())
    {
        if(adjacentRegion == toRegion)
        {
            return true;
        }
    }

    return false;
}

void BoardModel::mergeAllMovedTribes()
{
    foreach(RegionModel *regionModel, this->regions.values())
    {
        regionModel->mergeMovedTribes();
    }

    return;
}

void BoardModel::increaseEra()
{
    this->era++;
    emit eraChanged(this->era);
    return;
}

void BoardModel::populationGrowth()
{
    foreach(RegionModel *region, this->regions.values())
    {
        if(region->getTribes() > 0)
        {
            region->setTribes(region->getTribes()+1);
        }
    }

    return;
}

void BoardModel::moveTribes(int fromRegion, int toRegion, int howMany)
{
    this->regions[fromRegion]->setTribes(this->regions[fromRegion]->getTribes() - howMany);
    this->regions[toRegion]->addToMovedTribes(howMany);

    return;
}

bool BoardModel::bordersOnFrontier(int region)
{
    foreach(HexModel *hexModel, this->regionHexes[region])
    {
        foreach(HexModel *adjacentHex, hexModel->getAdjacentHexes().values())
        {
            if(adjacentHex->isFrontier())
            {
                return true;
            }
        }
    }

    return false;
}

bool BoardModel::bordersOnSea(int region)
{
    foreach(HexModel *hexModel, this->regionHexes[region])
    {
        foreach(HexModel *adjacentHex, hexModel->getAdjacentHexes().values())
        {
            if(adjacentHex->isSea())
            {
                return true;
            }
        }
    }

    return false;
}

bool BoardModel::bordersOnDesert(int region)
{
    foreach(HexModel *hexModel, this->regionHexes[region])
    {
        foreach(HexModel *adjacentHex, hexModel->getAdjacentHexes().values())
        {
            RegionModel *regionModel = this->refRegionModel(adjacentHex->getRegion());

            if(regionModel != NULL && regionModel->hasDesert())
            {
                return true;
            }
        }
    }

    return false;
}

void BoardModel::decimateUnsupportedTribes()
{
    foreach(RegionModel *regionModel, this->regions)
    {
        int tribes = regionModel->getTribes();
        int supportCount = regionModel->getTribeSupportCount();
        int unsupportedTribes = tribes - supportCount;

        if(unsupportedTribes > 0)
        {
            regionModel->setTribes(regionModel->getTribes() - unsupportedTribes);
        }
    }

    return;
}

void BoardModel::checkCitySupport()
{
    foreach(RegionModel *regionModel, this->regions.values())
    {
        if(regionModel->hasCity() && !regionModel->hasFarm())
        {
            regionModel->decreaseCityAV(1);
        }
    }
    return;
}

void BoardModel::decimateZeroAVCities()
{
    foreach(RegionModel *regionModel, this->regions.values())
    {
        regionModel->decimateZeroAVCity();
    }
    return;
}

const EventCard *BoardModel::drawCard(bool tell)
{
    const EventCard *card = this->eventCardsLeft.takeAt(Common::random() % this->eventCardsLeft.size());

    emit this->sendCardsLeftCount(this->eventCardsLeft.size());

    if(tell)
    {
        this->sendMessage("Drawing a card...");
        this->sendMessage(" ");
    }

    return card;
}

const EventCard *BoardModel::drawOriginalCard(bool tell)
{
    this->originalCard = this->drawCard(tell);
    return this->originalCard;
}

void BoardModel::reshuffleCards()
{
    this->eventCardsLeft = this->eventCards;
    emit this->sendCardsLeftCount(this->eventCardsLeft.size());
    return;
}

void BoardModel::setSelectRegion(int region, bool select)
{
    assert(this->regions.contains(region) && this->regionHexes.contains(region));

    this->regions[region]->setSelected(select);

    foreach(HexModel *hex, this->regionHexes[region])
    {
        hex->setSelected(select);
    }

    return;
}

void BoardModel::unselectAllRegions()
{
    foreach(int region, this->regions.keys())
    {
        this->setSelectRegion(region, false);
    }

    return;
}

void BoardModel::disableButtons()
{
    this->buildCity = false;
    this->buildFarm = false;
    this->expedition = false;
    this->aquireAdvances = false;
    this->buildWonder = false;
    this->collectTaxes = false;
    this->forestation = false;
    this->mining = false;
    this->doneEnabled = false;
    return;
}

void BoardModel::enableDoneButton()
{
    this->doneEnabled = true;
    return;
}

void BoardModel::enableMainPhaseButtons()
{
    this->doneEnabled = true;
    this->buildCity = true;
    this->buildFarm = true;
    this->expedition = true;
    this->aquireAdvances = true;
    this->buildWonder = true;
    return;
}

void BoardModel::decimateAllSelectedTribes()
{
    foreach(RegionModel *regionModel, this->regions.values())
    {
        regionModel->decimateSelectedTribes();
    }
    return;
}

void BoardModel::unselectAllSelectedTribes()
{
    foreach(RegionModel *regionModel, this->regions.values())
    {
        regionModel->setSelectedTribes(0);
    }
    return;
}

void BoardModel::addGold(int gold)
{
    this->setGold(this->gold + gold);
    return;
}

void BoardModel::removeGold(int gold)
{
    this->setGold(this->gold - gold);
    return;
}

void BoardModel::enableAllHexes()
{
    foreach(QList<HexModel *> hexes, this->hexModels)
    {
        foreach(HexModel *hex, hexes)
        {
            hex->setEnable(true);
        }
    }

    return;
}

void BoardModel::setUnsetHexesToSea()
{
    foreach(QList<HexModel *> hexes, this->hexModels)
    {
        foreach(HexModel *hex, hexes)
        {
            if(hex->getRegion() == -1 && !hex->isFrontier())
            {
                hex->setSea(true);
            }
        }
    }

    return;
}

void BoardModel::setChoosingHexesDone()
{
    foreach(QList<HexModel *> hexes, this->hexModels)
    {
        foreach(HexModel *hex, hexes)
        {
            hex->setBasePixmap(":/hex");
        }
    }

    return;
}

bool BoardModel::checkRegionHexSet(const QSet<HexModel *> &regionHexSet)
{
    if(regionHexSet.isEmpty())
    {
        return true;
    }

    HexModel *start = regionHexSet.values().first();
    QSet<HexModel *> checkedSet;
    checkedSet.insert(start);
    QSet<HexModel *> next = start->getAdjacentHexes().values().toSet().intersect(regionHexSet).subtract(checkedSet);

    while(!next.isEmpty())
    {
        HexModel *current = next.toList().first();
        next.remove(current);
        next = next.unite(current->getAdjacentHexes().values().toSet().intersect(regionHexSet).subtract(checkedSet).subtract(next));
        checkedSet.insert(current);
    }

    return regionHexSet.size() == checkedSet.size();
}

void BoardModel::deriveRegionHexes()
{
    assert(!this->hexModels.isEmpty());

    int region;

    for(int x = 0; x < this->hexModels.size(); ++x)
    {
        for(int y = 0; y < this->hexModels[0].size(); ++y)
        {
            region = this->hexModels[x][y]->getRegion();
            if(region >= 0)
            {
                if(this->hexModels[x][y]->isRepresentativeHex())
                {
                    this->hexModels[x][y]->setRegionModel(this->regions[region]);
                }
                else
                {
                    this->hexModels[x][y]->unsetRegionModel();
                }
                this->regionHexes[region].insert(this->hexModels[x][y]);
            }
        }
    }

    return;
}

void BoardModel::initializeCards()
{
    QMap<int, Event *> events1;
    events1.insert(1, new TribalWarEvent(this));
    events1.insert(2, new EpidemicEvent(0,0,1,this));
    events1.insert(3, new FamineEvent(this));
    events1.insert(4, new UprisingEvent(this));
    events1.insert(6, new VisitationEvent(0,0,2,BoardModel::FLOREN, this));
    events1.insert(8, new CorruptionEvent(0,0,1,this));
    QMap<int, Event *> events2;
    events2.insert(1, new VolcanoEvent(this));
    events2.insert(2, new TribalWarEvent(this));
    events2.insert(3, new CorruptionEvent(1,0,0,this));
    events2.insert(5, new CivilWarEvent(0,0,1,this));
    events2.insert(6, new VisitationEvent(1,1,1,BoardModel::GILDA, this));
    events2.insert(7, new EarthquakeEvent(2,1,0, this));
    events2.insert(8, new VisitationEvent(0,0,3,BoardModel::ATLANTEA, this));
    QMap<int, Event *> events3;
    events3.insert(2, new BanditsEvent(0,1,1,this));
    events3.insert(4, new EpidemicEvent(1,1,1,this));
    events3.insert(5, new VisitationEvent(0,3,0,BoardModel::ATLANTEA, this));
    events3.insert(7, new VisitationEvent(1,1,1,BoardModel::GILDA, this));
    events3.insert(8, new CivilWarEvent(0,0,1,this));
    QMap<int, Event *> events4;
    events4.insert(1, new EarthquakeEvent(1,0,0,this));
    events4.insert(3, new VisitationEvent(0,0,2,BoardModel::FLOREN, this));
    events4.insert(4, new SuperstitionEvent(1,0,0,this));
    events4.insert(6, new FamineEvent(this));
    events4.insert(7, new UprisingEvent(this));
    events4.insert(8, new VisitationEvent(0,1,2,BoardModel::NORDIG, this));
    QMap<int, Event *> events5;
    events5.insert(3, new EpidemicEvent(0,0,1,this));
    events5.insert(4, new VisitationEvent(0,0,2,BoardModel::GILDA, this));
    events5.insert(5, new CorruptionEvent(0,1,0,this));
    events5.insert(6, new TribalWarEvent(this));
    events5.insert(7, new CorruptionEvent(0,1,0,this));
    events5.insert(8, new FloodEvent(0,2,0,this));
    QMap<int, Event *> events6;
    events6.insert(1, new FloodEvent(1,0,0,this));
    events6.insert(3, new VisitationEvent(0,0,2,BoardModel::ATLANTEA, this));
    events6.insert(4, new CivilWarEvent(0,0,1,this));
    events6.insert(5, new SandstormEvent(this));
    events6.insert(6, new EpidemicEvent(1,0,1,this));
    events6.insert(8, new EarthquakeEvent(0,3,0,this));
    QMap<int, Event *> events7;
    events7.insert(2, new VisitationEvent(0,1,1,BoardModel::NORDIG, this));
    events7.insert(3, new AnarchyEvent(this));
    events7.insert(4, new VisitationEvent(0,0,2,BoardModel::FLOREN, this));
    events7.insert(5, new UprisingEvent(this));
    events7.insert(6, new AnarchyEvent(this));
    events7.insert(7, new BanditsEvent(0,0,2,this));
    QMap<int, Event *> events8;
    events8.insert(1, new SandstormEvent(this));
    events8.insert(2, new TribalWarEvent(this));
    events8.insert(3, new VisitationEvent(0,0,2,BoardModel::FLOREN, this));
    events8.insert(4, new FloodEvent(1,1,0,this));
    events8.insert(7, new VisitationEvent(0,1,2,BoardModel::ATLANTEA, this));
    events8.insert(8, new UprisingEvent(this));
    QMap<int, Event *> events9;
    events9.insert(2, new VolcanoEvent(this));
    events9.insert(4, new VisitationEvent(0,3,0,BoardModel::GILDA, this));
    events9.insert(5, new VolcanoEvent(this));
    events9.insert(6, new CorruptionEvent(0,1,0,this));
    events9.insert(7, new SuperstitionEvent(0,0,1,this));
    events9.insert(8, new SuperstitionEvent(0,0,1,this));
    QMap<int, Event *> events10;
    events10.insert(1, new FamineEvent(this));
    events10.insert(2, new FamineEvent(this));
    events10.insert(3, new SandstormEvent(this));
    events10.insert(5, new VisitationEvent(0,3,0,BoardModel::ATLANTEA, this));
    events10.insert(6, new UprisingEvent(this));
    events10.insert(8, new BanditsEvent(1,1,1,this));
    QMap<int, Event *> events11;
    events11.insert(2, new VisitationEvent(0,1,1,BoardModel::NORDIG, this));
    events11.insert(4, new BanditsEvent(0,0,2,this));
    events11.insert(5, new EarthquakeEvent(1,0,1,this));
    events11.insert(6, new BanditsEvent(0,1,2,this));
    events11.insert(7, new CivilWarEvent(0,0,1,this));
    events11.insert(8, new CorruptionEvent(0,1,0,this));
    QMap<int, Event *> events12;
    events12.insert(1, new VisitationEvent(0,0,1,BoardModel::GILDA, this));
    events12.insert(3, new VolcanoEvent(this));
    events12.insert(4, new AnarchyEvent(this));
    events12.insert(5, new FloodEvent(1,2,0,this));
    events12.insert(6, new VisitationEvent(0,1,2,BoardModel::ATLANTEA, this));
    events12.insert(7, new VolcanoEvent(this));
    QMap<int, Event *> events13;
    events13.insert(2, new EarthquakeEvent(2,0,0,this));
    events13.insert(3, new TribalWarEvent(this));
    events13.insert(5, new BanditsEvent(0,0,2,this));
    events13.insert(6, new CivilWarEvent(0,0,1,this));
    events13.insert(6, new VisitationEvent(0,1,2,BoardModel::NORDIG, this));
    events13.insert(8, new SuperstitionEvent(0,1,0,this));
    QMap<int, Event *> events14;
    events14.insert(1, new VisitationEvent(0,0,1,BoardModel::NORDIG, this));
    events14.insert(2, new EarthquakeEvent(0,2,0,this));
    events14.insert(4, new VisitationEvent(0,0,2,BoardModel::FLOREN, this));
    events14.insert(6, new VolcanoEvent(this));
    events14.insert(7, new FamineEvent(this));
    events14.insert(8, new AnarchyEvent(this));
    QMap<int, Event *> events15;
    events15.insert(2, new SandstormEvent(this));
    events15.insert(3, new FloodEvent(0,2,0,this));
    events15.insert(4, new SandstormEvent(this));
    events15.insert(5, new AnarchyEvent(this));
    events15.insert(6, new VisitationEvent(1,1,1,BoardModel::GILDA, this));
    events15.insert(7, new VisitationEvent(1,1,1,BoardModel::FLOREN, this));
    QMap<int, Event *> events16;
    events16.insert(1, new EpidemicEvent(0,1,0,this));
    events16.insert(2, new FloodEvent(0,1,0,this));
    events16.insert(4, new SuperstitionEvent(1,0,0,this));
    events16.insert(5, new VisitationEvent(0,0,2,BoardModel::NORDIG, this));
    events16.insert(7, new AnarchyEvent(this));
    events16.insert(8, new VisitationEvent(0,1,2,BoardModel::ATLANTEA, this));

    // NOTE: Is only testing.
//    events1.insert(1, new CorruptionEvent(0,0,1,this));
//    events2.insert(1, new CorruptionEvent(0,0,1,this));
//    events3.insert(1, new CorruptionEvent(0,0,1,this));
//    events4.insert(1, new CorruptionEvent(0,0,1,this));
//    events5.insert(1, new CorruptionEvent(0,0,1,this));
//    events6.insert(1, new CorruptionEvent(0,0,1,this));
//    events7.insert(1, new CorruptionEvent(0,0,1,this));
//    events8.insert(1, new CorruptionEvent(0,0,1,this));
//    events9.insert(1, new CorruptionEvent(0,0,1,this));
//    events10.insert(1, new CorruptionEvent(0,0,1,this));
//    events11.insert(1, new CorruptionEvent(0,0,1,this));
//    events12.insert(1, new CorruptionEvent(0,0,1,this));
//    events13.insert(1, new CorruptionEvent(0,0,1,this));
//    events14.insert(1, new CorruptionEvent(0,0,1,this));
//    events15.insert(1, new CorruptionEvent(0,0,1,this));
//    events16.insert(1, new CorruptionEvent(0,0,1,this));

    this->eventCards.append(new EventCard(1,7,6, true , 0, events1, this));
    this->eventCards.append(new EventCard(2,4,7, false, 2, events2, this));
    this->eventCards.append(new EventCard(3,5,8, false, 1, events3, this));
    this->eventCards.append(new EventCard(4,6,7, false, 2, events4, this));
    this->eventCards.append(new EventCard(5,3,9, true , 1, events5, this));
    this->eventCards.append(new EventCard(6,4,6, true , 0, events6, this));
    this->eventCards.append(new EventCard(7,5,7, false, 1, events7, this));
    this->eventCards.append(new EventCard(8,6,8, false, 0, events8, this));
    this->eventCards.append(new EventCard(1,7,9, true , 0, events9, this));
    this->eventCards.append(new EventCard(2,4,10,true , 0, events10, this));
    this->eventCards.append(new EventCard(3,5,7, false, 1, events11, this));
    this->eventCards.append(new EventCard(4,6,8, false, 3, events12, this));
    this->eventCards.append(new EventCard(5,3,9, true , 3, events13, this));
    this->eventCards.append(new EventCard(6,4,7, false, 0, events14, this));
    this->eventCards.append(new EventCard(7,5,8, false, 2, events15, this));
    this->eventCards.append(new EventCard(8,6,6, false, 2, events16, this));
    this->eventCardsLeft = this->eventCards;

    QList<QList<AdvanceModel::Advance> > prequisites;
    QList<QString> positive;
    QList<QString> negative;

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Build Farm"
                    "    Once per Round, don't decimate a Forest when creating a Farm.");
    advances.insert(AdvanceModel::AGRICULTURE,
                    new AdvanceModel(AdvanceModel::AGRICULTURE,
                                     "Agriculture",
                                     3,2,0,
                                     false, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::ENGINEERING);
    positive.append("+ Upkeep (Advance City AV)"
                    "    Maximum City AV is 4.");
    positive.append("+ Event (CIVIL WAR)"
                    "    Reduction of the City AV is reduced by 1.");
    positive.append("+ Event (ATTACK)"
                    "    Instead of reducing 1 City AV to reduce 5 Attacking Force, reduce 1 City AV to reduce 8 Attacking Force.");
    advances.insert(AdvanceModel::ARCHITECTURE,
                    new AdvanceModel(AdvanceModel::ARCHITECTURE,
                                     " Architecture",
                                     6,6,6,
                                     true, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::LITERACY);
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[1].append(AdvanceModel::MUSIC);
    positive.append("+ Event (CIVIL WAR)");
    positive.append("    Before applying the Colleteral Damage, sbstract 2 from Colleteral Damage. This is applied before Meditation is applied.");
    advances.insert(AdvanceModel::ARTS,
                    new AdvanceModel(AdvanceModel::ARTS,
                                     "Arts",
                                     6,6,6,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    negative.append("- Event (SUPERSTITION)"
                    "    When adding up the amount of Event Cards to discard, add 2 to the GREEN SQUARE Number.");
    advances.insert(AdvanceModel::ASTRONOMY,
                    new AdvanceModel(AdvanceModel::ASTRONOMY,
                                     "Astronomy",
                                     3,4,0,
                                     false, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Upkeep (After Advance City AV)"
                    "    If you have 3 Gold, gain 1 Gold.");
    advances.insert(AdvanceModel::BANKING,
                    new AdvanceModel(AdvanceModel::BANKING,
                                     "Banking",
                                     5,4,6,
                                     false, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Upkeep (Advance City AV)"
                    "    Reduce the decimation of Tribes by 1, when increasing the AV of a City.");
    advances.insert(AdvanceModel::BASIC_TOOLS,
                    new AdvanceModel(AdvanceModel::BASIC_TOOLS,
                                     "Basic Tools",
                                     1,0,0,
                                     false, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ New Main Action (Collect Taxes)"
                    "    Try to collect taxes."
                    "    1. Draw Event Card."
                    "    2. Add amount of Gold Nuggets shown."
                    "    3. If the Event Card has Handshaking on it, decimate your Gold by 1 and repeat from step 1. Otherwise, end the action. If you can't decimate 1 Gold, start an Anarchy Event.");
    positive.append("+ Aquire Advances"
                    "    If you aquire the Black Market, you get 5 Gold one time only.");
    negative.append("- Upkeep (Decimate Gold)"
                    "    When decimating all Coins, you must decimate at least 1 Gold or an Anarchy Event starts.");
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::CULTURE_OF_THIEVES);
    advances.insert(AdvanceModel::BLACK_MARKET,
                    new AdvanceModel(AdvanceModel::BLACK_MARKET,
                                     "Black\nMarket",
                                     1,0,0,
                                     false, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Upkeep (Reduce City AV)"
                    "    Use Farms in the EMPIRE to support Cities anywhere and not only in the Region the Farm is in. A Farm can still only support one City.");
    advances.insert(AdvanceModel::CARTAGE,
                    new AdvanceModel(AdvanceModel::CARTAGE,
                                     "Cartage",
                                     2,2,0,
                                     false, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Expedition"
                    "   Every tribe sent to an Expedition, counts as two tribes.");
    advances.insert(AdvanceModel::CAVALRY,
                    new AdvanceModel(AdvanceModel::CAVALRY,
                                     "Cavalry",
                                     8,6,8,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::MILITARY);
    positive.append("When aquiring this Advance, denote any City as the Capitol."
                    "If this city is decimated, choose another City as your Capitol."
                    "+ Upkeep (Advance City AV)"
                    "    A Capitol can have its AV increased to 10 AV."
                    "    Only Tribes in the Region of the Capitol can be decimated to pay for the advance of the Capitol."
                    "    Any increase higher than 4 AV additionally decimates a Forest, Mountain or Farm in the Region of the Capitol."
                    "    A Capitol can only be advanced once per Upkeep."
                    "    Basic Tools, Simple Tools and Machining don't apply for the Capitol.");
    advances.insert(AdvanceModel::CENTRALIZED_GOVERNMENT,
                    new AdvanceModel(AdvanceModel::CENTRALIZED_GOVERNMENT,
                                     "Centralized Government",
                                     10,6,8,
                                     true, true, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::DEMOCRACY);
    positive.append("+ Upkeep (Advance City AV)"
                    "    Increase the AV of one City this turn, without decimating Tribes."
                    "    This stacks with Masonry and Slave Labor.");
    positive.append("+ Event (CIVIL WAR)"
                    "    When reducing City AV, a City AV can't be reduced below 1.");
    positive.append("+ Event (TSUNAMI)"
                    "    Instead of decimating 1 Tribe per 1 Damage and reducing 1 City AV per 2 Damage, decimate 1 Tribe per 3 Damage and reduce 1 City AV per 5 Damage.");
    advances.insert(AdvanceModel::CIVIL_SERVICE,
                    new AdvanceModel(AdvanceModel::CIVIL_SERVICE,
                                     "Civil Service",
                                     15,8,6,
                                     true, true, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::ROADBUILDING);
    positive.append("+ Build City"
                    "    Only need 2 Tribes to build a city.");
    advances.insert(AdvanceModel::COMMON_TONGUE,
                    new AdvanceModel(AdvanceModel::COMMON_TONGUE,
                                     "Common Tongue",
                                     10,6,0,
                                     false, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Upkeep (Decimate Gold)"
                    "    Don't decimate any Gold but does not cancel Black Market.");
    advances.insert(AdvanceModel::COINAGE,
                    new AdvanceModel(AdvanceModel::COINAGE,
                                     "Coinage",
                                     3,2,3,
                                     false, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Upkeep (After Decimate Gold)"
                    "    1. Draw Event Card."
                    "    2. The RED CIRCLE Number is the Active Region."
                    "    3. If the Active Region has at least 1 Tribe, decimate 1 Tribe, Draw an Event Card and gain the amount of Gold Nuggets as Gold. "
                    "       Otherwise, nothing happens.");
    positive.append("+ Event (TRADE)"
                    "    After the trading is done, you may decide to steal from the visiting Empire."
                    "    1. Draw Event Card"
                    "    2. If any amount of Gold Nuggets are shown on this Event Card, add them to the Stash. Continue with 3."
                    "       Otherwise, if no Gold Nuggets are shown on this Event Card, the Event is over."
                    "    3. You can now either stop the event and collect the Gold in the Stash or steal some more and go to 4."
                    "    4. Draw Event Card."
                    "    5. If any amount of Gold Nuggets are shown on this Event Card, add them to the Stash. Continue with 3."
                    "       Otherwise, if no Gold Nuggets are shown continue to 6."
                    "    6. Lose half of the collected Gold in the Stash (Round Down)."
                    "    7. Draw Event Card."
                    "    8. The RED CIRCLE Number is the Active Region."
                    "    9. Draw Event Card"
                    "   10. Add up the GREEN SQUARE Number and BLUE HEX Number. This is the Attacking Force. Follow Up with ATTACK.");
    advances.insert(AdvanceModel::CULTURE_OF_THIEVES,
                    new AdvanceModel(AdvanceModel::CULTURE_OF_THIEVES,
                                     "Culture of Thieves",
                                     1,0,0,
                                     false, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::GOVERNMENT);
    positive.append("+ Event (BANDITS)"
                    "    After determening the Attacking Force, substract from it the BLUE HEX Number of the BANDITS Event Card.");
    advances.insert(AdvanceModel::DEMOCRACY,
                    new AdvanceModel(AdvanceModel::DEMOCRACY,
                                     "Democracy",
                                     12,4,6,
                                     true, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::GOVERNMENT);
    positive.append("+ Event (VISITATION)"
                    "    Before VISITATION begins, you may attempt to make the visiting Empire a Trading Partner."
                    "    1. Decimate any amount of Tribes and Gold. The total decimated amount is the Diplomatic Offer."
                    "    2. Draw Event Card."
                    "    3. If Diplomatic Offer >= RED CIRCLE Number, you are Trading Partners with the Visiting Empire. This is permanent for the rest of the game."
                    "    4. If this empire has become your trading partner, follow up every VISITATION of this empire with TRADE (including now)."
                    "       Otherwise, continue VISITATION normally.");
    advances.insert(AdvanceModel::DIPLOMACY,
                    new AdvanceModel(AdvanceModel::DIPLOMACY,
                                     "Diplomacy",
                                     10,4,1,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::AGRICULTURE);
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[1].append(AdvanceModel::HORTICULTURE);
    positive.append("+ Population Growth and Movement"
                    "    Move any Tribes to any Region. But not through frontiers or across seas.");
    positive.append("+ Event (ATTACK)"
                    "    Whenever the Attacking Force moves to a new Region with Tribes, reduce the Attacking Force by 2.");
    negative.append("- Event (BANDITS)"
                    "    After determening the Attacking Force, add to it the BLUE HEX Number of the BANDITS event card.");
    negative.append("- Event (EPIDEMIC)"
                    "    The Epidemic moves directly to any Region with tribes, instead of only moving to bordering regions with tribes.");
    advances.insert(AdvanceModel::EQUESTRIAN,
                    new AdvanceModel(AdvanceModel::EQUESTRIAN,
                                     "Equestrian",
                                     8,6,0,
                                     true, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::MASONRY);
    positive.append("+ Upkeep (Advance City AV)"
                    "    Maximum City AV is now 3.");
    positive.append("+ Event (EARTHQUAKE)"
                    "    Cities hit by the earthquake and about to be decimated, have their City AV set to 1.");
    positive.append("+ Event (VOLCANO)"
                    "    Cities hit by anything from this event can't be reduced below a City AV of 1 and any decimated Cities become Cities with City AV 1.");
    positive.append("+ Event (TSUNAMI)"
                    "    Instead of decimating 1 Wonder per 3 damage, decimate 1 Wonder per 10 damage.");
    advances.insert(AdvanceModel::ENGINEERING,
                    new AdvanceModel(AdvanceModel::ENGINEERING,
                                     "Engineering",
                                     5,3,0,
                                     true, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Population Growth and Movement"
                    "    Move any amount of tribes from one region to another bordering on the same sea. Reduce the group of moved tribes by 1.");
    positive.append("+ Upkeep (Reduce City AV)"
                    "    Cities in Regions bordering on SEA don't need Farms to support them.");
    advances.insert(AdvanceModel::FISHING,
                    new AdvanceModel(AdvanceModel::FISHING,
                                     "Fishing",
                                     6,6,0,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::SENSE_OF_COMMUNITY);
    positive.append("+ Event (TRIBAL WAR)"
                    "    Skip this event.");
    negative.append("- Event (CORRUPTION)"
                    "    Before applying the Corruption, add 3 to the Corruption. This is applied before the Literacy advance is applied.");
    advances.insert(AdvanceModel::GOVERNMENT,
                    new AdvanceModel(AdvanceModel::GOVERNMENT,
                                     "Government",
                                     8,4,0,
                                     false, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ New Main Action (Forestation)"
                    "    Grow a Forest in a Region."
                    "    Decimate 4 Tribes in a Region to create a Forest there. The amount of tribes left after Forestation have to be at least 1.");
    advances.insert(AdvanceModel::HORTICULTURE,
                    new AdvanceModel(AdvanceModel::HORTICULTURE,
                                     "Horticulture",
                                     8,2,0,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::AGRICULTURE);
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[1].append(AdvanceModel::HORTICULTURE);
    positive.append("+ Event (FAMINE)"
                    "    Instead of decimating the Farm and reducing the City AV by 2, don't decimate the Farm and reduce the City AV by 1.");
    positive.append("+ Event (FLOOD)"
                    "    Instead of decimating the Farm and reducing the City AV by 1, don't decimate the Farm and don't reduce the City AV. Still place a Forest in that Region, if there is none.");
    positive.append("+ Event (SANDSTORM)"
                    "    Do not decimate any Farms in this event.");
    advances.insert(AdvanceModel::IRRIGATION,
                    new AdvanceModel(AdvanceModel::IRRIGATION,
                                     "Irrigation",
                                     10,2,2,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::GOVERNMENT);
    prequisites[0].append(AdvanceModel::PHILOSOPHY);
    positive.append("+ Event (ANARCHY)"
                    "    Instead of reducing City AV, reduce the Tribes in every Region by the CityAV in that region."
                    "    This cancels the effect of Machining.");
    positive.append("+ Event (BANDITS)"
                    "    After determening the Attacking Force, substract from it the BLUE HEX Number of the BANDITS event card.");
    positive.append("+ Event (CIVIL WAR)"
                    "    Instead of using the BLUE HEX Number to determine the Collateral Damage, use the GREEN SQUARE Number.");
    positive.append("+ Event (CORRUPTION)"
                    "    Don't decimate any gold in this event.");
    positive.append("+ Event (UPRISING)"
                    "    Decrease the amount of City AV to be reduced by 1.");
    advances.insert(AdvanceModel::LAW,
                    new AdvanceModel(AdvanceModel::LAW,
                                     "Law",
                                     10,5,6,
                                     false, true, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Event (ANARCHY)"
                    "    Reduce Tribes by 5 instead of 3.");
    positive.append("+ Event (CORRUPTION)"
                    "    Before applying it, divide the Corruption by 2 (Round Down)."
                    "    Do this after applying Government.");
    advances.insert(AdvanceModel::LITERACY,
                    new AdvanceModel(AdvanceModel::LITERACY,
                                     "Literacy",
                                     3,2,0,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::ENGINEERING);
    prequisites[0].append(AdvanceModel::METAL_WORKING);
    positive.append("+ Aquire Advances"
                    "    A Mountain/Volcano can now create wood instead of stone and a Forest may create stone instead of wood.");
    positive.append("+ Upkeep (Advance City AV)"
                    "    Increasing the City AV costs 1 less tribe.");
    negative.append("- Event (ANARCHY)"
                    "    Reduce 2 additional City AV in a City of your choice."
                    "    Law cancels this effect.");
    negative.append("- Event (UPRISING)"
                    "    Increase the amount of City AV to be reduced by 1.");
    advances.insert(AdvanceModel::MACHINING,
                    new AdvanceModel(AdvanceModel::MACHINING,
                                     "Machining",
                                     12,8,10,
                                     false, true, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Upkeep (Advance City AV)"
                    "       Increase the AV of one City this turn, without decimating Tribes."
                    "       This stacks with Civil Service and Slave Labor.");
    advances.insert(AdvanceModel::MASONRY,
                    new AdvanceModel(AdvanceModel::MASONRY,
                                     "Masonry",
                                     3,2,0,
                                     false, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::METAL_WORKING);
    positive.append("+ Population Growth and Movement"
                    "    Don't Reduce Tribe Group by 1 Tribe, when moving across the sea with Fishing or Navigation.");
    positive.append("+ Expedition"
                    "    Divide the BLUE HEX (or GREEN HEX, if using Navigation for sea Expeditions) Number by 2 (Round Up).");
    advances.insert(AdvanceModel::MAGNETICS,
                    new AdvanceModel(AdvanceModel::MAGNETICS,
                                     "Magnetics",
                                     12,6,0,
                                     false, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Event (CIVIL WAR)"
                    "    After applying all the Colleteral Damage, add 1 Tribe to every affected Region.");
    positive.append("+ Event (EARTHQUAKE)"
                    "    After the Event is over, create 2 Tribes in each affected Region that had at least 1 Tribe decimated.");
    positive.append("+ Event (EPIDEMIC)"
                    "    Before applying the Population Loss, divide it by 2 (Round down).");
    positive.append("+ Event (VOLCANO)"
                    "   After the Event is over, place 1 Tribe in each Region affected by this Event.");
    positive.append("+ Event (TSUNAMI)"
                    "    Add 2 Tribes in every Region affected by this event.");
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::LITERACY);
    advances.insert(AdvanceModel::MEDICINE,
                    new AdvanceModel(AdvanceModel::MEDICINE,
                                     "Medicine",
                                     8,3,1,
                                     true, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::MYTHOLOGY);
    positive.append("+ Event (CIVIL WAR)"
                    "    Before applying the Colleteral Damage, divide it by 2 (Round up). Apply this after Arts and Theater.");
    positive.append("+ Event (SUPERSTITION)"
                    "    You may end the event, if the Event Deck has only 1 Card left, while discarding the Event Cards.");
    advances.insert(AdvanceModel::MEDITATION,
                    new AdvanceModel(AdvanceModel::MEDITATION,
                                     "Meditation",
                                     4,3,0,
                                     true, false, true,
                                     prequisites,
                                     positive, negative));
    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::MINING);
    positive.append("+ Upkeep (Advance City AV)"
                    "    Maximum City AV is 3.");
    positive.append("+ Event (ATTACK)"
                    "    When reducing Tribes and Attacking Forces, reduce 1 additional Attacking Force for every Tribe reduced.");
    advances.insert(AdvanceModel::METAL_WORKING,
                    new AdvanceModel(AdvanceModel::METAL_WORKING,
                                     "Metal Working",
                                     7,3,0,
                                     false, true, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::GOVERNMENT);
    positive.append("+ Event Follow Up (ATTACK)"
                    "    When reducing Tribes and Attacking Forces, reduce 1 additional Attacking Force for every Tribe reduced.");
    negative.append("- Event (CIVIL WAR)"
                    "    Instead of reducing the City AV from the Active Region and the bordering Regions by 2, you reduce them by 3.");
    negative.append("- Event (UPRISING)"
                    "    After the Event ends, this happens:"
                    "    1. Select a bordering Region with a City."
                    "       If there is none, the Event is over."
                    "    2. Reduce the Region's City AV by 1 in the selected bordering Region. If the City AV is 0, decimate the City."
                    "    3. Decimate all Tribes in the selected bordering Region."
                    "       Organized Religion also applies here: "
                    "       Instead of decimating all Tribes, reduce the Tribes by 2.");
    advances.insert(AdvanceModel::MILITARY,
                    new AdvanceModel(AdvanceModel::MILITARY,
                                     "Military",
                                     8,4,3,
                                     false, true, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ New Main Action (Mining)"
                    "    Mine for Gold."
                    "    1. Decimate 3 Tribes on a Region with a Mountain or Volcano."
                    "       At least 1 Tribe has to remain."
                    "    2. Draw Event Card."
                    "    3. If there are Gold Nuggets on the Event Card, add that amount your Mine Cart."
                    "       Otherwise, the Mine Cart and all the gold it contained is lost and the action ends."
                    "    4. You can now either collect all the Gold from the Mine Cart or go to 2. to try to mine more gold.");
    advances.insert(AdvanceModel::MINING,
                    new AdvanceModel(AdvanceModel::MINING,
                                     "Mining",
                                     5,1,0,
                                     false, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::ORGANIZED_RELIGION);
    positive.append("+ Event (ATTACK)"
                    "    Before starting the ATTACK:"
                    "    1. Draw Event Card."
                    "    2. Reduce the Attacking Force by the GREEN SQUARE Number and gain as much gold as Attacking Forces actually reduced this way.");
    advances.insert(AdvanceModel::MINISTRY,
                    new AdvanceModel(AdvanceModel::MINISTRY,
                                     "Ministry",
                                     10,2,8,
                                     true, true, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Event (TRIBAL WAR)"
                    "    Instead of selecting 2 bordering Regions and reducing Tribes by 3 in the Active Region, select 1 bordering Region and reduce Tribes by 2 in the Active Region.");
    advances.insert(AdvanceModel::MUSIC,
                    new AdvanceModel(AdvanceModel::MUSIC,
                                     "Music",
                                     3,1,0,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    negative.append("- Event (CIVIL WAR)"
                    "    Instead of reducing the City AV from the Active Region and the bordering Regions by 2, you reduce them by 3.");
    advances.insert(AdvanceModel::MYTHOLOGY,
                    new AdvanceModel(AdvanceModel::MYTHOLOGY,
                                     "Mythology",
                                     2,2,0,
                                     false, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Expedition"
                    "    Instead of sending Tribes into the Frontier, you may send them to Sea and the BLUE HEX Number replaces GREEN HEX Number to determine how much gold you receive.");
    positive.append("+ Population Growth and Movement"
                    "    Move any amount of tribes from one region to another bordering on the same sea. Reduce the group of moved tribes by 1.");
    advances.insert(AdvanceModel::NAVIGATION,
                    new AdvanceModel(AdvanceModel::NAVIGATION,
                                     "Navigation",
                                     5,3,0,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::MYTHOLOGY);
    positive.append("+ Event (ANARCHY)"
                    "    Instead of all Regions with Cities, choose 4 Regions with or without cities and apply the event only to the chosen Regions with cities.");
    positive.append("+ Event (UPRISING)"
                    "    Instead of decimating all the Tribes, reduce Tribes by 2.");
    advances.insert(AdvanceModel::ORGANIZED_RELIGION,
                    new AdvanceModel(AdvanceModel::ORGANIZED_RELIGION,
                                     "Organized Religion",
                                     8,6,6,
                                     true, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::ARTS);
    prequisites[0].append(AdvanceModel::THEATER);
    positive.append("+ Upkeep (After Advance City and Banking)"
                    "    For any city gain gold equal to City AV minus 4. If the gold gained is 0 or less, you gain no gold.");
    advances.insert(AdvanceModel::PATRONAGE,
                    new AdvanceModel(AdvanceModel::PATRONAGE,
                                     "Patronage",
                                     8,6,5,
                                     false, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::MEDITATION);
    positive.append("+ Event (VISITATION)"
                    "    You may draw a second Event Card to attempt to get a Handshake.");
    advances.insert(AdvanceModel::PHILOSOPHY,
                    new AdvanceModel(AdvanceModel::PHILOSOPHY,
                                     "Philosophy",
                                     12,6,0,
                                     false, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::CARTAGE);
    positive.append("+ Population Growth and Movement"
                    "    Repeat moving tribes once.");
    positive.append("+ Expedition"
                    "    Send additional Tribes from bordering Regions into the same Expedition.");
    negative.append("- Event (EPIDEMIC)"
                    "    The Epidemic moves directly to any Region with tribes, instead of only moving to bordering regions with tribes."
                    "    Additionally, if all tribes are decimated in a region as a result of this event, reduce the City AV by 2 in that region. If the City AV becomes 0, decimate the City.");
    advances.insert(AdvanceModel::ROADBUILDING,
                    new AdvanceModel(AdvanceModel::ROADBUILDING,
                                     "Road-\nbuilding",
                                     6,1,0,
                                     true, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::NAVIGATION);
    prequisites[0].append(AdvanceModel::ASTRONOMY);
    positive.append("+ Expedition"
                    "    Every tribe sent to an Expedition on sea, counts as two tribes.");
    advances.insert(AdvanceModel::SAILS_AND_RIGGINGS,
                    new AdvanceModel(AdvanceModel::SAILS_AND_RIGGINGS,
                                     "Sails and Riggings",
                                     6,6,4,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::EQUESTRIAN);
    positive.append("+ Event (TRIBAL WAR)"
                    "    Instead of multiplying the Warring Tribes by 2, don't do that.");
    advances.insert(AdvanceModel::SENSE_OF_COMMUNITY,
                    new AdvanceModel(AdvanceModel::SENSE_OF_COMMUNITY,
                                     "Sense of Community",
                                     6,6,8,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::NAVIGATION);
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[1].append(AdvanceModel::FISHING);
    positive.append("+ Event (TRADE)"
                    "    If the Card you drew for the Gold you receive also has a Handshake on it, draw another Event Card and increase the amount of Gold you receive by the GREEN SQUARE Number on that card.");
    advances.insert(AdvanceModel::SHIPPING,
                    new AdvanceModel(AdvanceModel::SHIPPING,
                                     "Shipping",
                                     8,4,2,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Upkeep (Advance City AV)"
                    "    Increasing the City AV costs one less tribe.");
    advances.insert(AdvanceModel::SIMPLE_TOOLS,
                    new AdvanceModel(AdvanceModel::SIMPLE_TOOLS,
                                     "Simple\nTools",
                                     4,4,0,
                                     true, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Upkeep (Advance City AV)"
                    "    Increase the AV of one City this turn, without decimating Tribes."
                    "    This stacks with Civil Service and Masonry.");
    negative.append("- Event (ANARCHY)"
                    "    After applying the effects of ANARCHY, do this:"
                    "    1. Draw Event Card."
                    "    2. Reduce Tribes in the Empire according to the amount of the RED CIRCLE Number. The amount can be distributed however you want in the EMPIRE.");
    negative.append("- Event (BANDITS)"
                    "    After determening the Attacking Force, add to it the BLUE HEX Number of the BANDITS event card.");
    negative.append("- Event (UPRISING)"
                    "    Additionally, decimate the Farm in all Regions that have no Cities.");
    advances.insert(AdvanceModel::SLAVE_LABOR,
                    new AdvanceModel(AdvanceModel::SLAVE_LABOR,
                                     "Slave Labor",
                                     1,0,0,
                                     false, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    positive.append("+ Aquire Advances"
                    "    Pay one less Tribe when Aquiring: Mythology, Music, Literacy, Philosophy, Arts, Theater, Organized Religion or Law.");
    advances.insert(AdvanceModel::STORY_TELLING,
                    new AdvanceModel(AdvanceModel::STORY_TELLING,
                                     "Story\nTelling",
                                     2,4,0,
                                     false, false, true,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::MINING);
    positive.append("+ Mining"
                    "    If the mine cart is lost with any gold in it, gain 1 gold anyway.");
    advances.insert(AdvanceModel::SURVEYING,
                    new AdvanceModel(AdvanceModel::SURVEYING,
                                     "Surveying",
                                     5,2,4,
                                     true, true, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::LITERACY);
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[1].append(AdvanceModel::MUSIC);
    positive.append("+ Event (CIVIL WAR)"
                    "    Before applying the Colleteral Damage, substract 2 from it."
                    "    This is applied before Meditation.");
    advances.insert(AdvanceModel::THEATER,
                    new AdvanceModel(AdvanceModel::THEATER,
                                     "Theater",
                                     10,6,3,
                                     true, false, false,
                                     prequisites,
                                     positive, negative));

    prequisites.clear();
    positive.clear();
    negative.clear();
    prequisites.append(QList<AdvanceModel::Advance>());
    prequisites[0].append(AdvanceModel::LITERACY);
    positive.append("+ End of Era"
                    "    When determening the Glory, chose 4 more advances than there are tribes in the Empire.");
    advances.insert(AdvanceModel::WRITTEN_RECORD,
                    new AdvanceModel(AdvanceModel::WRITTEN_RECORD,
                                     "Written Record",
                                     5,2,1,
                                     true, true, false,
                                     prequisites,
                                     positive, negative));

    return;
}

int BoardModel::getWidth() const
{
    return this->hexModels.size();
}

int BoardModel::getHeight() const
{
    if(this->hexModels.isEmpty())
    {
        return 0;
    }

    return this->hexModels.first().size();
}

QMap<int, QSet<HexModel *> > BoardModel::getRegionHexes() const
{
    return this->regionHexes;
}

QMap<int, RegionModel *> BoardModel::getRegions() const
{
    return this->regions;
}

QMap<int, RegionModel *> BoardModel::getAdjacentRegions(int fromRegion) const
{
    QMap<int, RegionModel *> result;

    foreach(HexModel *hex, this->regionHexes[fromRegion])
    {
        foreach(HexModel *adjacent, hex->getAdjacentHexes().values())
        {
            int region = adjacent->getRegion();
            if(region >= 0 && !result.contains(region) && fromRegion != region)
            {
                result.insert(region, this->refRegionModel(region));
            }
        }
    }

    return result;
}

QMap<int, RegionModel *> BoardModel::getMountainRegions() const
{
    QMap<int, RegionModel *> result;

    foreach(int key, this->regions.keys())
    {
        if(this->regions[key]->hasMountain())
        {
            result.insert(key, this->regions[key]);
        }
    }

    return result;
}

QMap<int, RegionModel *> BoardModel::getForestRegions() const
{
    QMap<int, RegionModel *> result;

    foreach(int key, this->regions.keys())
    {
        if(this->regions[key]->hasForest())
        {
            result.insert(key, this->regions[key]);
        }
    }

    return result;
}

int BoardModel::getMountainCount() const
{
    int result = 0;

    foreach(RegionModel *regionModel, this->regions.values())
    {
        if(regionModel->hasMountain())
        {
            result++;
        }
    }

    return result;
}

int BoardModel::getForestCount() const
{
    int result = 0;

    foreach(RegionModel *regionModel, this->regions.values())
    {
        if(regionModel->hasForest())
        {
            result++;
        }
    }

    return result;
}

int BoardModel::getDesertCount() const
{
    int result = 0;

    foreach(RegionModel *regionModel, this->regions.values())
    {
        if(regionModel->hasDesert())
        {
            result++;
        }
    }

    return result;
}

int BoardModel::getTribeCount() const
{
    int result = 0;

    foreach(RegionModel *regionModel, this->regions.values())
    {
        result += regionModel->getTribes();
    }

    return result;
}

int BoardModel::getCityCount() const
{
    int result = 0;

    foreach(RegionModel *regionModel, this->regions.values())
    {
        if(regionModel->hasCity())
        {
            result++;
        }
    }

    return result;
}

QMap<int, RegionModel *> BoardModel::getSelectedRegions() const
{
    QMap<int, RegionModel *> result;

    foreach(int key, this->regions.keys())
    {
        if(this->regions[key]->isSelected())
        {
            result.insert(key, this->regions[key]);
        }
    }

    return result;
}

bool BoardModel::canBuildCity() const
{
    return this->buildCity;
}

bool BoardModel::hasCity() const
{
    foreach(RegionModel *regionModel, this->regions.values())
    {
        if(regionModel->hasCity())
        {
            return true;
        }
    }

    return false;
}

bool BoardModel::canBuildFarm() const
{
    return this->buildFarm;
}

bool BoardModel::canDoExpedition() const
{
    return this->expedition;
}

bool BoardModel::canAquireAdvance() const
{
    return this->aquireAdvances;
}

bool BoardModel::canBuildWonder() const
{
    return this->buildWonder;
}

bool BoardModel::canCollectTaxes() const
{
    return this->collectTaxes;
}

bool BoardModel::canDoForestation() const
{
    return this->forestation;
}

bool BoardModel::canDoMining() const
{
    return this->mining;
}

bool BoardModel::isDoneEnabled() const
{
    return this->doneEnabled;
}

int BoardModel::getEra() const
{
    return this->era;
}

int BoardModel::getLastEra() const
{
    return this->lastEra;
}

bool BoardModel::isEndOfEra() const
{
    return this->eventCardsLeft.isEmpty();
}

int BoardModel::getAllMovedTribes() const
{
    int result = 0;

    foreach(RegionModel *regionModel, this->regions.values())
    {
        result += regionModel->getMovedTribes();
    }

    return result;
}

int BoardModel::getAllSelectedTribes() const
{
    int result = 0;

    foreach(RegionModel *regionModel, this->regions.values())
    {
        result += regionModel->getSelectedTribes();
    }

    return result;
}

int BoardModel::getGold() const
{
    return this->gold;
}

int BoardModel::getGloryScore() const
{
    return this->gloryScore;
}

bool BoardModel::hasAdvanceAquired(AdvanceModel::Advance advance) const
{
    return this->advancesAquired.contains(advance);
}

QSet<AdvanceModel::Advance> BoardModel::getAdvancesAquired() const
{
    return this->advancesAquired;
}

void BoardModel::setActiveRegion(int region, bool isBad)
{
    this->unsetActiveRegion();
    this->activeRegion = this->regions.value(region, NULL);

    if(this->activeRegion != NULL)
    {
        QSet<HexModel *> selectedHexes = this->getRegionHexes().value(this->activeRegion->getRegion());

        foreach(HexModel *regionModel, selectedHexes)
        {
            regionModel->setActive(true, isBad);
        }
    }

    return;
}

void BoardModel::unsetActiveRegion()
{
    if(this->activeRegion != NULL)
    {
        QSet<HexModel *> selectedHexes = this->getRegionHexes().value(this->activeRegion->getRegion());

        foreach(HexModel *regionModel, selectedHexes)
        {
            regionModel->setActive(false);
        }
    }

    this->activeRegion = NULL;
    return;
}

void BoardModel::setEra(int era)
{
    this->era = era;
    emit this->eraChanged(this->era);
}

void BoardModel::setGold(int gold)
{
    this->gold = gold;
    emit this->goldChanged(this->gold);
    return;
}

void BoardModel::setGloryScore(int gloryScore)
{
    this->gloryScore = gloryScore;
    emit this->gloryScoreChanged(this->gloryScore);
    return;
}

void BoardModel::setAdvanceAquired(AdvanceModel::Advance advance)
{
    this->advancesAquired.insert(advance);
    emit this->advanceAquired(advance);
    return;
}

HexModel *BoardModel::refHexModel(int x, int y)
{
    assert(x >= 0 && x < this->hexModels.size());
    assert(y >= 0 && y < this->hexModels[x].size());

    return this->hexModels[x][y];
}

RegionModel *BoardModel::refRegionModel(int x, int y)
{
    return this->refRegionModel(this->refHexModel(x,y)->getRegion());
}

RegionModel *BoardModel::refRegionModel(int region) const
{
    if(region < 0)
    {
        return NULL;
    }

    return this->regions.value(region, NULL);
}

RegionModel *BoardModel::refActiveRegion() const
{
    return this->activeRegion;
}

const EventCard *BoardModel::refOriginalCard() const
{
    return this->originalCard;
}

const AdvanceModel *BoardModel::refAdvanceModel(AdvanceModel::Advance advance) const
{
    assert(this->advances.contains(advance));
    return this->advances[advance];
}

void BoardModel::clearBoard()
{
    this->regionHexes.clear();

    foreach(QList<HexModel *> list, this->hexModels)
    {
        for(int i = 0; i < list.size(); ++i)
        {
            HexModel *hexModel = list.at(i);
            if(hexModel != NULL)
            {
                hexModel->deleteLater();
            }
        }
    }

    this->hexModels.clear();

    emit this->boardCleared();
}

void BoardModel::serialize(QDataStream &writer) const
{
    assert(!this->hexModels.isEmpty());

    int width = this->hexModels.size();
    int height = this->hexModels[0].size();

    writer << width;
    writer << height;

    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            this->hexModels[x][y]->serialize(writer);
        }
    }

    QList<int> regionKeys = this->regions.keys();
    int regionCount = regionKeys.size();

    writer << regionCount;

    for(int i = 0; i < regionCount; ++i)
    {
        writer << regionKeys[i];
        this->regions[regionKeys[i]]->serialize(writer);
    }

    int eventCardsLeftCount = this->eventCardsLeft.size();
    writer << eventCardsLeftCount;

    for(int i = 0; i < eventCardsLeftCount; ++i)
    {
        writer << this->eventCards.indexOf(this->eventCardsLeft[i]);
    }

    int advancesAquiredCount = this->advancesAquired.size();
    writer << advancesAquiredCount;

    QList<AdvanceModel::Advance> advanceList = this->advancesAquired.toList();

    for(int i = 0; i < advancesAquiredCount; ++i)
    {
        writer << (int) advanceList[i];
    }

    writer << this->buildCity;
    writer << this->buildFarm;
    writer << this->expedition;
    writer << this->aquireAdvances;
    writer << this->buildWonder;
    writer << this->collectTaxes;
    writer << this->forestation;
    writer << this->mining;
    writer << this->doneEnabled;

    writer << this->gold;

    writer << this->era;
    writer << this->lastEra;

    writer << this->gloryScore;

    return;
}

void BoardModel::deserialize(QDataStream &reader)
{
    int width;
    reader >> width;
    int height;
    reader >> height;

    this->newBoard(width, height);

    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            this->hexModels[x][y]->deserialize(reader);
        }
    }

    int regionCount;
    reader >> regionCount;
    int regionKey;

    for(int i = 0; i < regionCount; ++i)
    {
        reader >> regionKey;
        if(!this->regions.contains(regionKey))
        {
            this->regions[regionKey] = new RegionModel(regionKey);
        }
        this->regions[regionKey]->deserialize(reader);
    }

    int eventCardsLeftCount;
    reader >> eventCardsLeftCount;
    int index;

    this->eventCardsLeft.clear();

    for(int i = 0; i < eventCardsLeftCount; ++i)
    {
        reader >> index;
        this->eventCardsLeft.append(this->eventCards[index]);
    }

    int advancesAquiredCount;
    reader >> advancesAquiredCount;

    int advanceID;
    this->advancesAquired.clear();

    for(int i = 0; i < advancesAquiredCount; ++i)
    {
        reader >> advanceID;
        this->advancesAquired.insert(AdvanceModel::Advance(advanceID));
    }

    reader >> this->buildCity;
    reader >> this->buildFarm;
    reader >> this->expedition;
    reader >> this->aquireAdvances;
    reader >> this->buildWonder;
    reader >> this->collectTaxes;
    reader >> this->forestation;
    reader >> this->mining;
    reader >> this->doneEnabled;

    reader >> this->gold;

    reader >> this->era;
    reader >> this->lastEra;

    reader >> this->gloryScore;

    this->groupSeas();
    this->deriveRegionHexes();

    emit this->boardUpdated();
    return;
}
