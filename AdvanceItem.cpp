#include "AdvanceItem.hpp"

#include <QPainter>

AdvanceItem::AdvanceItem(qreal xPos,
                         qreal yPos,
                         BoardModel *boardModel,
                         AdvanceModel::Advance advance,
                         QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      advance(advance),
      boardModel(boardModel),
      selected(false)
{
    this->setPos(xPos, yPos);
    this->setShapeMode(AdvanceItem::MaskShape);
    this->updateAdvanceItem();
}

void AdvanceItem::updateAdvanceItem()
{
    const AdvanceModel *advanceModel = this->boardModel->refAdvanceModel(this->advance);
    QPixmap result(":/advance_base");

    QPainter painter(&result);

    painter.setPen(QColor(0,0,0));

    QFont font = painter.font();
    font.setFamily("Sans");
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRect(15,3,37,12),QString::number(advanceModel->getVictoryPoints()), QTextOption(Qt::AlignCenter));
    painter.drawText(QRect(1,9,83,47),advanceModel->getName(), QTextOption(Qt::AlignCenter));

    font.setPointSize(6);
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(QRect(58,4,90,12),QString::number(advanceModel->getTribesCost()), QTextOption(Qt::AlignCenter));
    painter.drawText(QRect(80,4,114,12),QString::number(advanceModel->getGoldCost()), QTextOption(Qt::AlignCenter));

    if(advanceModel->getRequiresWood())
    {
        painter.drawPixmap(1,1,QPixmap(":/advance_wood"));
    }

    if(advanceModel->getRequiresStone())
    {
        painter.drawPixmap(1,1,QPixmap(":/advance_stone"));
    }

    if(advanceModel->getRequiresFood())
    {
        painter.drawPixmap(1,1,QPixmap(":/advance_food"));
    }

    int pos = 0;

    for(int i = 0; i < advanceModel->getPositiveEffects().size(); ++i)
    {
        painter.drawPixmap(86+(12*pos),35,QPixmap(":/advance_positive"));
        pos++;
    }

    for(int i = 0; i < advanceModel->getNegativeEffects().size(); ++i)
    {
        painter.drawPixmap(86+(12*pos),35,QPixmap(":/advance_negative"));
        pos++;
    }

    this->setPixmap(result);
    return;
}

void AdvanceItem::toggleSelected()
{
    this->selected = !this->selected;
    return;
}

void AdvanceItem::setSelected(bool selected)
{
    this->selected = selected;
    return;
}

bool AdvanceItem::isSelected() const
{
    return this->selected;
}

