#include "BoardModel.hpp"

#include <QMap>
#include <QDebug>

#include "Common.hpp"

BoardModel::BoardModel(int width, int height, QObject *parent)
    : QObject(parent)
{
    this->newBoard(width, height);
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
        RegionModel *regionModel = new RegionModel(hex);
        hex->setRepresentativeHex(true, regionModel);
        this->regions.insert(region, regionModel);
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

RegionModel *BoardModel::refRegionModel(int region)
{
    if(region < 0)
    {
        return NULL;
    }

    return this->regions.value(region, NULL);
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
