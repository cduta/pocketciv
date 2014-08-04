#include "HexItem.hpp"

#include <QPainter>

#include <iostream>

#include <boost/assert.hpp>

HexItem::HexItem(qreal xPos, qreal yPos, HexModel *hexModel, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    this->hexModel = hexModel;
    this->setPos(HexItem::BOARD_BORDER + xPos, HexItem::BOARD_BORDER + yPos);
    this->setShapeMode(HexItem::MaskShape);
    this->updateHexItem();
}

void HexItem::drawHexBase()
{
    assert(this->hexModel != NULL);

    int visibleBorders = this->hexModel->getVisibleBorders();
    bool enabled = this->hexModel->isEnabled();
    bool showRegionNumber = this->hexModel->showRegionNumber();
    int region = this->hexModel->getRegion();
    bool isFrontier = this->hexModel->isFrontier();
    bool isSea = this->hexModel->isSea();
    bool isRepresentative = this->hexModel->isRepresentativeHex();

    QPixmap result(this->hexModel->getBasePixmap());

    QPainter painter(&result);

    this->setEnabled(enabled);

    if(isSea)
    {
        painter.drawPixmap(0,0,QPixmap(":/sea"));
    }
    else if(isFrontier)
    {
        painter.drawPixmap(0,0,QPixmap(":/frontier"));
    }
    else if(isRepresentative)
    {
        painter.drawPixmap(0,0,QPixmap(":/hex_representative"));

        RegionModel *regionModel = this->hexModel->refRegionModel();
        int tribeCount = regionModel->getTribes();

        if(regionModel->hasMountain())
        {
            painter.drawPixmap(0,0,QPixmap(":/mountain"));
        }

        if(regionModel->hasForest())
        {
            painter.drawPixmap(0,0,QPixmap(":/forest"));
        }

        if(regionModel->hasDesert())
        {
            painter.drawPixmap(0,0,QPixmap(":/desert"));
        }

        if(tribeCount > 0)
        {
            painter.drawPixmap(0,0,QPixmap(":/tribe"));

            painter.setPen(QColor(0,0,0));
            QFont font = painter.font();
            font.setFamily("Sans");
            font.setPointSize(6);
            painter.setFont(font);
            painter.drawText(QRect(21,3,14,8),QString::number(tribeCount), QTextOption(Qt::AlignCenter));
            painter.setPen(HEX_BORDER_COLOR);
        }
    }

    painter.setPen(HEX_BORDER_COLOR);

    if((visibleBorders & DRAW_UPPER_LEFT_BORDER) != 0)
    {
        painter.drawPixmap(0,0,QPixmap(":/upper_left"));
    }

    if((visibleBorders & DRAW_UPPER_CENTER_BORDER) != 0)
    {
        painter.drawPixmap(14,0,QPixmap(":/center"));
    }

    if((visibleBorders & DRAW_UPPER_RIGHT_BORDER) != 0)
    {
        painter.drawPixmap(35,0,QPixmap(":/upper_right"));
    }

    if((visibleBorders & DRAW_LOWER_RIGHT_BORDER) != 0)
    {
        painter.drawPixmap(35,21,QPixmap(":/lower_right"));
    }

    if((visibleBorders & DRAW_LOWER_CENTER_BORDER) != 0)
    {
        painter.drawPixmap(14,41,QPixmap(":/center"));
    }

    if((visibleBorders & DRAW_LOWER_LEFT_BORDER)!= 0)
    {
        painter.drawPixmap(0,21,QPixmap(":/lower_left"));
    }

    if(showRegionNumber)
    {
        painter.setPen(QColor(0,0,0));
        QFont font = painter.font();
        font.setFamily("Sans");
        font.setBold(true);
        font.setPointSize(10);
        painter.setFont(font);
        painter.drawText(result.rect(),QString::number(region), QTextOption(Qt::AlignCenter));
        painter.setPen(HEX_BORDER_COLOR);
    }

    if(!this->isEnabled())
    {
        painter.drawPixmap(0,0,QPixmap(":/hex_disabled"));
    }

    this->setPixmap(result);
}

void HexItem::updateHexItem()
{
    this->drawHexBase();
}

void HexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->hexModel->triggerHex(event->button());
    return;
}
