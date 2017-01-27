#include "AdvancesTable.hpp"

#include <QHeaderView>

AdvancesTable::AdvancesTable(BoardModel *boardModel, const QMap<AdvanceModel::Advance, AdvanceItem *> &advanceItemMap, QWidget *parent)
    : QTableWidget(parent), boardModel(boardModel), advanceItemMap(advanceItemMap)
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setColumnCount(11);
    QStringList headers;
    headers << "State" << "Name" << "Victory Points" << "Tribes" << "Gold" << "Wood" << "Stone" << "Food" << "Advance Prequisites" << "Positives" << "Negatives";
    this->setHorizontalHeaderLabels(headers);
    this->horizontalHeader()->setSortIndicatorShown(true);
    this->setSortingEnabled(true);
    this->resizeColumnToContents(0);
    this->resizeColumnToContents(1);
    this->resizeColumnToContents(2);
    this->resizeColumnToContents(3);
    this->resizeColumnToContents(5);
    this->resizeColumnToContents(6);
    this->resizeColumnToContents(7);
    this->resizeColumnToContents(8);
    this->resizeColumnToContents(9);
    this->resizeColumnToContents(10);

    this->setRowCount(this->advanceItemMap.count());
    QList<AdvanceModel::Advance> advances = this->advanceItemMap.keys();

    QColor blue(0x17,0x57,0xe8);
    QColor red(0xea,0x1a,0x1a);
    QColor green(0x16,0xb7,0x16);

    for(int i = 0; i < advances.count(); ++i)
    {
        AdvanceModel::Advance advance = advances[i];
        const AdvanceModel *advanceModel = this->boardModel->refAdvanceModel(advance);
        QTableWidgetItem *aquirableItem = new QTableWidgetItem("");
        QTableWidgetItem *nameItem = new QTableWidgetItem(advanceModel->getName());
        IntegerTableItem *tribeCostItem = new IntegerTableItem(advanceModel->getTribesCost());
        IntegerTableItem *goldCostItem = new IntegerTableItem(advanceModel->getGoldCost());
        QTableWidgetItem *woodItem = new QTableWidgetItem(advanceModel->getRequiresWood() ? "Yes" : "No");
        QTableWidgetItem *stoneItem = new QTableWidgetItem(advanceModel->getRequiresStone() ? "Yes" : "No");
        QTableWidgetItem *foodItem = new QTableWidgetItem(advanceModel->getRequiresFood() ? "Yes" : "No");
        QTableWidgetItem *advanceRequirementsItem = new QTableWidgetItem("");

        this->advanceTableItems.insert(advance, nameItem);

        if(advanceModel->advancePrequisitesMet(this->boardModel->getAdvancesAquired()))
        {
            if(advanceModel->getPrequisites().count() == 0)
            {
                advanceRequirementsItem->setText("None");
            }
            else
            {
                advanceRequirementsItem->setText("Satisfied");
            }
        }
        else
        {
            advanceRequirementsItem->setText("Incomplete");
        }

        if(this->boardModel->hasAdvanceAquired(advance))
        {
            aquirableItem->setText("Aquired");
        }
        else
        {
            aquirableItem->setText("-");
        }

        RegionModel *activeRegion = this->boardModel->refActiveRegion();

        if(!this->boardModel->hasAdvanceAquired(advance) && activeRegion != NULL && this->boardModel->isAquiringAdvances())
        {
            if(this->boardModel->hasAdvanceAquired(advance))
            {
                aquirableItem->setText("Aquired");
                aquirableItem->setBackgroundColor(blue);
            }
            if(this->boardModel->canAquireAdvance(advance))
            {
                aquirableItem->setText("Affordable");
                aquirableItem->setBackgroundColor(green);
            }
            else
            {
                aquirableItem->setText("-");
                aquirableItem->setBackgroundColor(red);
            }

            int tribesCost = advanceModel->getTribesCost();

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::STORY_TELLING) && AdvanceModel::hasStoryTellingDiscount(advance))
            {
                tribesCost--;
            }

            if(tribesCost <= activeRegion->getTribes())
            {
                tribeCostItem->setBackgroundColor(green);
            }
            else
            {
                tribeCostItem->setBackgroundColor(red);
            }

            if(advanceModel->getGoldCost() <= this->boardModel->getGold())
            {
                goldCostItem->setBackgroundColor(green);
            }
            else
            {
                goldCostItem->setBackgroundColor(red);
            }

            if(!advanceModel->getRequiresWood() || activeRegion->hasForest())
            {
                woodItem->setBackgroundColor(green);
            }
            else
            {
                woodItem->setBackgroundColor(red);
            }

            if(!advanceModel->getRequiresStone() || activeRegion->hasMountain() || activeRegion->hasVolcano())
            {
                stoneItem->setBackgroundColor(green);
            }
            else
            {
                stoneItem->setBackgroundColor(red);
            }

            if(!advanceModel->getRequiresFood() || activeRegion->hasFarm())
            {
                foodItem->setBackgroundColor(green);
            }
            else
            {
                foodItem->setBackgroundColor(red);
            }

            if(advanceModel->advancePrequisitesMet(this->boardModel->getAdvancesAquired()))
            {
                advanceRequirementsItem->setBackgroundColor(green);
            }
            else
            {
                advanceRequirementsItem->setBackgroundColor(red);
            }
        }

        this->setItem(i, 0, aquirableItem);
        this->setItem(i, 1, nameItem);
        this->setItem(i, 2, new IntegerTableItem(advanceModel->getVictoryPoints()));
        this->setItem(i, 3, tribeCostItem);
        this->setItem(i, 4, goldCostItem);
        this->setItem(i, 5, woodItem);
        this->setItem(i, 6, stoneItem);
        this->setItem(i, 7, foodItem);
        this->setItem(i, 8, advanceRequirementsItem);
        this->setItem(i, 9, new IntegerTableItem(advanceModel->getPositiveEffects().count()));
        this->setItem(i, 10, new IntegerTableItem(advanceModel->getNegativeEffects().count()));
    }

    this->resizeColumnToContents(0);
    this->resizeColumnToContents(1);
    this->resizeColumnToContents(2);
    this->resizeColumnToContents(3);
    this->resizeColumnToContents(5);
    this->resizeColumnToContents(6);
    this->resizeColumnToContents(7);
    this->resizeColumnToContents(8);
    this->resizeColumnToContents(9);
    this->resizeColumnToContents(10);

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectAdvance()));
}

void AdvancesTable::selectAdvance()
{
    if(this->selectedItems().isEmpty() ||
       !this->item(this->currentRow(), 1))
    {
        return;
    }

    QTableWidgetItem *item = this->item(this->currentRow(), 1);
    this->advanceItemMap[this->advanceTableItems.key(item)]->showDescriptionDialog();

    return;
}
