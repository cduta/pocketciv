#include "AdvanceItem.hpp"

AdvanceItem::AdvanceItem(qreal xPos,
                         qreal yPos,
                         BoardModel *boardModel,
                         AdvanceModel::Advance advance,
                         QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      advance(advance),
      boardModel(boardModel)
{
    this->setPos(xPos, yPos);
    this->setShapeMode(AdvanceItem::MaskShape);
    this->updateAdvanceItem();
}

void AdvanceItem::updateAdvanceItem()
{
    QPixmap result(":/advance_base");
    this->setPixmap(result);
    return;
}

