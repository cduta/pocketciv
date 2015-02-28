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
