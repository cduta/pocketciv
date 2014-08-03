#include "HexModel.hpp"

#include <boost/assert.hpp>

HexModel::HexModel(int xPos, int yPos, bool enable, int visibleBorders, QObject *parent)
    : QObject(parent), xPos(xPos), yPos(yPos), region(-1), enable(enable), visibleBorders(visibleBorders), regionNumberShown(false)
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

bool HexModel::showRegionNumber() const
{
    return this->regionNumberShown;
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
