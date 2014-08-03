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

    QPixmap result(":/hex_generate");

    QPainter painter(&result);

    this->setEnabled(enabled);

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

    if(!this->isEnabled())
    {
        painter.drawPixmap(0,0,QPixmap(":/hex_disabled"));
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
//    this->hexModel->setVisibleBorders(DRAW_UPPER_LEFT_BORDER | DRAW_UPPER_CENTER_BORDER | DRAW_UPPER_RIGHT_BORDER |
//                                      DRAW_LOWER_LEFT_BORDER | DRAW_LOWER_CENTER_BORDER | DRAW_LOWER_RIGHT_BORDER);
}
