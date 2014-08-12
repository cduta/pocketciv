#include "BoardModel.hpp"

#include <QMap>
#include <QDebug>

#include "Common.hpp"
#include "Event/EpidemicEvent.hpp"

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
    QMap<int, Event *> events;
    events.insert(1, new EpidemicEvent(0,1,0,this));
    this->eventCards.insert(new EventCard(1,7,6, events, this));
    this->eventCards.insert(new EventCard(2,4,7, events, this));
    this->eventCards.insert(new EventCard(3,5,8, events, this));
    this->eventCards.insert(new EventCard(4,6,7, events, this));
    this->eventCards.insert(new EventCard(5,3,9, events, this));
    this->eventCards.insert(new EventCard(6,4,6, events, this));
    this->eventCards.insert(new EventCard(7,5,7, events, this));
    this->eventCards.insert(new EventCard(8,6,8, events, this));
    this->eventCards.insert(new EventCard(1,7,9, events, this));
    this->eventCards.insert(new EventCard(2,4,10, events, this));
    this->eventCards.insert(new EventCard(3,5,7, events, this));
    this->eventCards.insert(new EventCard(4,6,8, events, this));
    this->eventCards.insert(new EventCard(5,3,9, events, this));
    this->eventCards.insert(new EventCard(6,4,7, events, this));
    this->eventCards.insert(new EventCard(7,5,8, events, this));
    this->eventCards.insert(new EventCard(8,6,6, events, this));
    this->eventCardsLeft = this->eventCards.toList();
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
    this->eventCardsLeft = this->eventCards.toList();
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

int BoardModel::getGloryScore() const
{
    return this->gloryScore;
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

void BoardModel::setGold(int gold)
{
    this->gold = gold;
    emit goldChanged(this->gold);
    return;
}

void BoardModel::setGloryScore(int gloryScore)
{
    this->gloryScore = gloryScore;
    emit this->gloryScoreChanged(this->gloryScore);
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

void BoardModel::clearBoard()
{
    this->regionHexes.clear();

    foreach(QList<HexModel *> list, this->hexModels)
    {
        foreach(HexModel *hexModel, list)
        {
            hexModel->deleteLater();
        }
    }

    this->hexModels.clear();

    emit this->boardCleared();
}
