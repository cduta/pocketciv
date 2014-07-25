#include "HexItem.hpp"

#include <QPainter>
#include <iostream>

HexItem::HexItem(qreal xPos, qreal yPos, HexModel *hexModel, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    this->hexModel = hexModel;
    this->updateHexItem();
    this->setPos(HexItem::BOARD_BORDER + xPos, HexItem::BOARD_BORDER + yPos);
    this->setShapeMode(HexItem::MaskShape);
}

void HexItem::drawHexItem(int visibleBorders, bool enabled)
{
    QPixmap result(":/hex");
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

    this->setPixmap(result);
}

void HexItem::updateHexItem()
{
    this->drawHexItem(this->hexModel->getVisibleBorders(), this->hexModel->isEnabled());
}

void HexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    this->hexModel->setVisibleBorders(DRAW_UPPER_LEFT_BORDER | DRAW_UPPER_CENTER_BORDER | DRAW_UPPER_RIGHT_BORDER |
                                      DRAW_LOWER_LEFT_BORDER | DRAW_LOWER_CENTER_BORDER | DRAW_LOWER_RIGHT_BORDER);
}
