#include "AdvanceItem.hpp"

#include <QPainter>
#include <QScrollBar>

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

    this->layout = new QGridLayout(&this->descriptionDialog);
    this->descriptionDialog.setLayout(this->layout);

    this->description = new QPlainTextEdit(&this->descriptionDialog);
    QFont font("monospace");
    this->description->setFont(font);
    this->description->setReadOnly(true);
    this->layout->addWidget(this->description, 0,0);
    this->descriptionDialog.resize(400,400);

    this->updateAdvanceItem();
}

void AdvanceItem::updateDesription()
{
    const AdvanceModel *advanceModel = this->boardModel->refAdvanceModel(this->advance);
    this->descriptionDialog.setWindowTitle(QString("%1 (%2 VP)").arg(advanceModel->getName()).arg(QString::number(advanceModel->getVictoryPoints())));

    QString wood;

    if(advanceModel->getRequiresWood())
    {
        wood = "Yes";
    }
    else
    {
        wood = "No";
    }

    QString stone;

    if(advanceModel->getRequiresStone())
    {
        stone = "Yes";
    }
    else
    {
        stone = "No";
    }

    QString food;

    if(advanceModel->getRequiresFood())
    {
        food = "Yes";
    }
    else
    {
        food = "No";
    }

    this->description->clear();
    this->description->insertPlainText(QString("Tribes Cost                       : %1\n").arg(QString::number(advanceModel->getTribesCost())));
    this->description->insertPlainText(QString("Gold Cost                         : %1\n").arg(QString::number(advanceModel->getGoldCost())));
    this->description->insertPlainText(QString("Wood (Forest) required            : %1\n").arg(wood));
    this->description->insertPlainText(QString("Stone (Mountain/Volcano) required : %1\n").arg(stone));
    this->description->insertPlainText(QString("Food (Farm) required              : %1\n\n").arg(food));
    this->description->insertPlainText(QString("Effects\n%1").arg(advanceModel->getEffects()));

    return;
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

    this->updateDesription();
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

void AdvanceItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
    }
    else if(event->button() == Qt::RightButton)
    {
        this->descriptionDialog.show();
        this->description->verticalScrollBar()->setValue(0);
    }
    return;
}

