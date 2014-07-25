#include "HexModel.hpp"

#include <boost/assert.hpp>

HexModel::HexModel(int xPos, int yPos, bool enable, int visibleBorders, QObject *parent)
    : QObject(parent), xPos(xPos), yPos(yPos), enable(enable), visibleBorders(visibleBorders)
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
    typedef QPair<int, HexModel *> TypePair;
    foreach(TypePair pair, this->adjacentHexes)
    {
        int adjacentBorder = pair.first;
        HexModel *hex = pair.second;

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

void HexModel::setAdjacentHexes(QList<QPair<int, HexModel *> > &adjacentHexes)
{
    this->adjacentHexes = adjacentHexes;
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
