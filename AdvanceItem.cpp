#include "AdvanceItem.hpp"

#include <QPainter>

AdvanceItem::AdvanceItem(qreal xPos,
                         qreal yPos,
                         BoardModel *boardModel,
                         AdvanceModel::Advance advance,
                         AdvanceItemType advanceType,
                         QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      advance(advance),
      boardModel(boardModel),
      advanceType(advanceType)
{
    assert(this->advanceType != AQUIRE || this->boardModel->refActiveRegion() != NULL);

    this->setPos(xPos, yPos);
    this->setShapeMode(AdvanceItem::MaskShape);

    this->descriptionDialog.advance = this->advance;
    this->descriptionDialog.boardModel = this->boardModel;

    QFont font("monospace");
    this->descriptionDialog.description->setFont(font);
    this->descriptionDialog.description->setReadOnly(true);

    if(this->advanceType == AQUIRE)
    {
        this->descriptionDialog.layout->addWidget(this->descriptionDialog.description, 0,0,1,2);
        this->descriptionDialog.layout->addWidget(this->descriptionDialog.aquireAdvance, 1,0,1,2);
        this->descriptionDialog.cancel->hide();

        QObject::connect(&this->descriptionDialog, SIGNAL(advanceAquired(AdvanceModel::Advance)), this->boardModel, SLOT(aquireAdvance(AdvanceModel::Advance)));
    }
    else
    {
        this->descriptionDialog.layout->addWidget(this->descriptionDialog.description, 0,0);
        this->descriptionDialog.cancel->hide();
        this->descriptionDialog.aquireAdvance->hide();
    }

    this->descriptionDialog.dialog.resize(400,400);

    this->updateAdvanceItem();
}

void AdvanceItem::updateDesription()
{
    const AdvanceModel *advanceModel = this->boardModel->refAdvanceModel(this->advance);
    this->descriptionDialog.dialog.setWindowTitle(QString("%1 (%2 VP)").arg(advanceModel->getName()).arg(QString::number(advanceModel->getVictoryPoints())));

    QString wood;

    if(advanceModel->getRequiresWood())
    {
        wood = "Yes";

        if(this->advanceType == AQUIRE)
        {
            if(this->boardModel->refActiveRegion()->hasForest() ||
               (this->boardModel->hasAdvanceAquired(AdvanceModel::MACHINING) && !advanceModel->getRequiresStone() &&
                (this->boardModel->refActiveRegion()->hasMountain() || this->boardModel->refActiveRegion()->hasVolcano())))
            {
                wood.append(" (OK)");
            }
            else
            {
                wood.append(" (  )");
            }
        }
    }
    else
    {
        wood = "No";
    }

    QString stone;

    if(advanceModel->getRequiresStone())
    {
        stone = "Yes";

        if(this->advanceType == AQUIRE)
        {
            if(this->boardModel->refActiveRegion()->hasMountain() || this->boardModel->refActiveRegion()->hasVolcano() ||
               (this->boardModel->hasAdvanceAquired(AdvanceModel::MACHINING) && !advanceModel->getRequiresWood() && this->boardModel->refActiveRegion()->hasForest()))
            {
                stone.append(" (OK)");
            }
            else
            {
                stone.append(" (  )");
            }
        }
    }
    else
    {
        stone = "No";
    }

    QString food;

    if(advanceModel->getRequiresFood())
    {
        food = "Yes";

        if(this->advanceType == AQUIRE)
        {
            if(this->boardModel->refActiveRegion()->hasFarm())
            {
                food.append(" (OK)");
            }
            else
            {
                food.append(" (  )");
            }
        }
    }
    else
    {
        food = "No";
    }

    int tribesCost = advanceModel->getTribesCost();

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::STORY_TELLING) && AdvanceModel::hasStoryTellingDiscount(advance))
    {
        tribesCost--;
    }

    QString tcString = QString::number(tribesCost);
    QString gcString = QString::number(advanceModel->getGoldCost());

    if(this->advanceType == AQUIRE)
    {
        tcString.append(QString(" / %1").arg(this->boardModel->refActiveRegion()->getTribes()));
        gcString.append(QString(" / %1").arg(this->boardModel->getGold()));
    }

    this->descriptionDialog.description->clear();
    this->descriptionDialog.description->insertPlainText(QString("Tribes Cost                       : %1\n").arg(tcString));
    this->descriptionDialog.description->insertPlainText(QString("Gold Cost                         : %1\n").arg(gcString));
    this->descriptionDialog.description->insertPlainText(QString("Wood (Forest) required            : %1\n").arg(wood));
    this->descriptionDialog.description->insertPlainText(QString("Stone (Mountain/Volcano) required : %1\n").arg(stone));
    this->descriptionDialog.description->insertPlainText(QString("Food (Farm) required              : %1\n\n").arg(food));
    this->descriptionDialog.description->insertPlainText(QString("Effects\n%1").arg(advanceModel->getEffects()));

    return;
}

void AdvanceItem::updateAdvanceItem()
{
    const AdvanceModel *advanceModel = this->boardModel->refAdvanceModel(this->advance);
    QPixmap result(148,50);

    QPainter painter(&result);
    QPen oldPen = painter.pen();
    QPen bluePen = painter.pen();
    QColor blue(0x17,0x57,0xe8);
    bluePen.setColor(blue);

    QPen redPen = painter.pen();
    QColor red(0xea,0x1a,0x1a);
    redPen.setColor(red);

    QPen greenPen = painter.pen();
    QColor green(0x16,0xb7,0x16);
    greenPen.setColor(green);

    QPen yellowPen = painter.pen();
    QColor yellow(0xd7,0xd5,0x19);
    yellowPen.setColor(yellow);

    QPen greyPen = painter.pen();
    QColor grey(0xc1,0xc1,0xc1);
    greyPen.setColor(grey);

    QColor backgroundBlue(0x9e,0xdc,0xf1);
    QColor backgroundGreen(0x79,0xd9,0x75);

    painter.fillRect(result.rect(), QColor(255,255,255));

    if(this->advanceType == OVERVIEW)
    {
        if(this->boardModel->getAdvancesAquired().contains(this->advance))
        {
            painter.fillRect(result.rect(), backgroundBlue);

            painter.setPen(bluePen);
            QRect rect(result.rect().x(), result.rect().y(), result.rect().width()-1, result.rect().height()-1);
            painter.drawRect(rect);
            painter.setPen(oldPen);
        }
    }

    if(this->advanceType == SELECTABLE)
    {
        if(this->boardModel->hasAquiredAdvanceSelected(this->advance))
        {
            painter.fillRect(result.rect(), backgroundGreen);

            painter.setPen(greenPen);
            QRect rect(result.rect().x(), result.rect().y(), result.rect().width()-1, result.rect().height()-1);
            painter.drawRect(rect);
            painter.setPen(oldPen);
        }
        else
        {
            if(this->boardModel->getAdvancesAquired().contains(this->advance))
            {
                painter.fillRect(result.rect(), backgroundBlue);

                painter.setPen(bluePen);
                QRect rect(result.rect().x(), result.rect().y(), result.rect().width()-1, result.rect().height()-1);
                painter.drawRect(rect);
                painter.setPen(oldPen);
            }
            else
            {
                painter.fillRect(result.rect(), grey);
            }
        }
    }

    if(this->advanceType == AQUIRE)
    {
        RegionModel *activeRegion = this->boardModel->refActiveRegion();
        assert(activeRegion != NULL);

        if(this->boardModel->getAdvancesAquired().contains(this->advance))
        {
            painter.fillRect(result.rect(), backgroundBlue);

            painter.setPen(bluePen);
            QRect rect(result.rect().x(), result.rect().y(), result.rect().width()-1, result.rect().height()-1);
            painter.drawRect(rect);
            painter.setPen(oldPen);
            this->descriptionDialog.aquireAdvance->setEnabled(false);
        }
        else
        {
            if(advanceModel->advanceRequirementsMet(this->boardModel->getAdvancesAquired()) &&
               !activeRegion->hasAdvanceAquired())
            {
                bool isAvailable = true;

                QRect r(85,3,29,14);

                int tribesCost = advanceModel->getTribesCost();

                if(this->boardModel->hasAdvanceAquired(AdvanceModel::STORY_TELLING) && AdvanceModel::hasStoryTellingDiscount(advance))
                {
                    tribesCost--;
                }

                if(tribesCost <= activeRegion->getTribes() &&
                   tribesCost < this->boardModel->getTribeCount())
                {
                    painter.fillRect(r, green);
                }
                else
                {
                    painter.fillRect(r, red);
                    isAvailable = false;
                }

                r = QRect(115,3,30,14);
                if(advanceModel->getGoldCost() <= this->boardModel->getGold())
                {
                    painter.fillRect(r, green);
                }
                else
                {
                    painter.fillRect(r, red);
                    isAvailable = false;
                }

                r = QRect(85,18,19,14);
                if(!advanceModel->getRequiresWood() || activeRegion->hasForest())
                {
                    painter.fillRect(r, green);
                }
                else
                {
                    painter.fillRect(r, red);
                    isAvailable = false;
                }

                r = QRect(105,18,19,14);
                if(!advanceModel->getRequiresStone() || activeRegion->hasMountain() || activeRegion->hasVolcano())
                {
                    painter.fillRect(r, green);
                }
                else
                {
                    painter.fillRect(r, red);
                    isAvailable = false;
                }

                r = QRect(125,18,20,14);
                if(!advanceModel->getRequiresFood() || activeRegion->hasFarm())
                {
                    painter.fillRect(r, green);
                }
                else
                {
                    painter.fillRect(r, red);
                    isAvailable = false;
                }

                if(isAvailable)
                {
                    painter.setPen(greenPen);
                    this->descriptionDialog.aquireAdvance->setEnabled(true);
                }
                else
                {
                    painter.setPen(redPen);
                    this->descriptionDialog.aquireAdvance->setEnabled(false);
                }

                QRect rect(result.rect().x(), result.rect().y(), result.rect().width()-1, result.rect().height()-1);
                painter.drawRect(rect);
                painter.setPen(oldPen);
            }
            else
            {
                painter.fillRect(result.rect(), grey);
                this->descriptionDialog.aquireAdvance->setEnabled(false);
            }
        }
    }

    painter.drawPixmap(0,0,QPixmap(":/advance_base"));

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

    int tribesCost = advanceModel->getTribesCost();

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::STORY_TELLING) && AdvanceModel::hasStoryTellingDiscount(advance))
    {
        tribesCost--;
    }

    painter.drawText(QRect(58,4,90,12),QString::number(tribesCost), QTextOption(Qt::AlignCenter));
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

void AdvanceItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(this->advanceType == AdvanceItem::SELECTABLE)
        {
            this->boardModel->toggleSelectAquiredAdvance(this->advance);
            this->updateAdvanceItem();
        }
        else
        {
            this->descriptionDialog.show();
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        this->descriptionDialog.show();
    }
    return;
}
