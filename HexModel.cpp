#include "HexModel.hpp"

#include <boost/assert.hpp>

#include <QSet>

HexModel::HexModel(int xPos, int yPos, bool enable, int visibleBorders, QObject *parent)
    : QObject(parent),
      xPos(xPos),
      yPos(yPos),
      region(-1),
      enable(enable),
      selected(false),
      active(false),
      bad(false),
      visibleBorders(visibleBorders),
      regionNumberShown(false),
      frontier(false),
      sea(false),
      basePixmap(":/hex_generate"),
      representativeHex(false),
      regionModel(NULL)
{}

void HexModel::addVisibleBorders(int visibleBorders)
{
    this->privateAddVisibleBorders(visibleBorders);
    this->updateHex();
}

void HexModel::removeVisibleBorders(int visibleBorders)
{
    this->privateRemoveVisibleBorders(visibleBorders);
    this->updateHex();
}

void HexModel::toggleFrontier()
{
    this->frontier = !this->frontier;
    return;
}

void HexModel::updateHex()
{
    foreach(int adjacentBorder, this->adjacentHexes.keys())
    {
        HexModel *hex = this->adjacentHexes[adjacentBorder];
        if((adjacentBorder & this->visibleBorders) != 0)
        {
            hex->privateAddVisibleBorders(oppositeBorder(adjacentBorder));
        }
        else
        {
            hex->privateRemoveVisibleBorders(oppositeBorder(adjacentBorder));
        }

        emit hexModelUpdated(hex->xPos, hex->yPos);
    }

    emit hexModelUpdated(this->xPos, this->yPos);
}

bool HexModel::isEnabled() const
{
    return this->enable;
}

int HexModel::getVisibleBorders() const
{
    return this->visibleBorders;
}

int HexModel::x() const
{
    return this->xPos;
}

int HexModel::y() const
{
    return this->yPos;
}

int HexModel::getRegion() const
{
    return this->region;
}

QMap<int, HexModel *> HexModel::getAdjacentHexes() const
{
    return this->adjacentHexes;
}

QSet<HexModel *> HexModel::getAdjacentSeaHexes() const
{
    QSet<HexModel *> result;
    foreach(HexModel *hex, this->adjacentHexes.values())
    {
        if(hex->isSea())
        {
            result.insert(hex);
        }
    }
    return result;
}

bool HexModel::showRegionNumber() const
{
    return this->regionNumberShown;
}

bool HexModel::isFrontier() const
{
    return this->frontier;
}

bool HexModel::isSea() const
{
    return this->sea;
}

QString HexModel::getBasePixmap() const
{
    return this->basePixmap;
}

bool HexModel::isRepresentativeHex() const
{
    return this->representativeHex;
}

bool HexModel::isSelected() const
{
    return this->selected;
}

bool HexModel::isActive() const
{
    return this->active;
}

bool HexModel::activeIsBad() const
{
    return this->bad;
}

void HexModel::setEnable(bool enable)
{
    this->enable = enable;
    this->updateHex();
    return;
}

void HexModel::setVisibleBorders(int visibleBorders)
{
    this->visibleBorders = visibleBorders;
    this->updateHex();
    return;
}

void HexModel::setAdjacentHexes(QMap<int, HexModel *> &adjacentHexes)
{
    assert(adjacentHexes.size() >= 0 && adjacentHexes.size() <= 6);
    this->adjacentHexes = adjacentHexes;
    return;
}

void HexModel::setRegion(int region)
{
    this->region = region;

    if(this->region > 0)
    {
        foreach(int border, this->adjacentHexes.keys())
        {
            if(this->adjacentHexes[border]->getRegion() != this->region)
            {
                this->addVisibleBorders(border);
            }
            if(this->adjacentHexes[border]->getRegion() == this->region)
            {
                this->removeVisibleBorders(border);
            }
        }
    }
    else if(this->region == -1)
    {
        this->unsetRegion();
    }

    this->updateHex();

    return;
}

void HexModel::unsetRegion()
{
    foreach(int border, this->adjacentHexes.keys())
    {
        if(this->adjacentHexes[border]->getRegion() == this->region)
        {
            this->addVisibleBorders(border);
        }
        else if(this->adjacentHexes[border]->getRegion() == -1)
        {
            this->removeVisibleBorders(border);
        }
    }

    this->region = -1;
    this->updateHex();

    return;
}

void HexModel::setRegionNumberShown(bool show)
{
    this->regionNumberShown = show;
    return;
}

void HexModel::setFrontier(bool frontier)
{
    this->frontier = frontier;
    return;
}

void HexModel::setSea(bool sea)
{
    this->sea = sea;
    return;
}

void HexModel::setBasePixmap(const QString &basePixmap)
{
    this->basePixmap = basePixmap;
    return;
}

void HexModel::setRepresentativeHex(bool representative, RegionModel *regionModel)
{
    this->representativeHex = representative;
    this->regionModel = regionModel;
    return;
}

void HexModel::setSelected(bool selected)
{
    this->selected = selected;
    return;
}

void HexModel::setActive(bool active, bool isBad)
{
    this->bad = isBad;
    this->active = active;
    return;
}

RegionModel *HexModel::refRegionModel()
{
    assert(this->regionModel != NULL && this->representativeHex);
    return this->regionModel;
}

void HexModel::triggerHex(Qt::MouseButton button)
{
    emit this->hexTriggered(button, this->xPos, this->yPos);
    return;
}

void HexModel::privateAddVisibleBorders(int visibleBorders)
{
    this->visibleBorders = this->visibleBorders | visibleBorders;
    return;
}

void HexModel::privateRemoveVisibleBorders(int visibleBorders)
{
    this->visibleBorders = this->visibleBorders & ~visibleBorders;
    return;
}
