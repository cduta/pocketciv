#ifndef HEXITEM_HPP
#define HEXITEM_HPP

#include <QList>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include "HexModel.hpp"

const QColor HEX_BORDER_COLOR = QColor(0x5f,0x5f,0x5f);
const QColor UNUSED_HEX_BORDER_COLOR = QColor(0x5f+0x40,0x5f+0x40,0x5f+0x40);

class HexItem : public QGraphicsPixmapItem
{
public:
    static const int BOARD_BORDER = 30;

private:
    HexModel *hexModel;

public:
    HexItem(qreal xPos, qreal yPos, HexModel *hexModel, QGraphicsItem *parent = 0);
    void updateHexItem();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void drawHexBase();
};

#endif // HEXITEM_HPP
