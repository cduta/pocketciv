#include "WondersTable.hpp"

#include <QAbstractItemView>
#include <QHeaderView>

WondersTable::WondersTable(BoardModel *boardModel, QMap<WonderModel::Wonder, int> wonders, WonderDescription::WonderDescriptionType wonderDescriptionType, QWidget *parent)
    : QTableWidget(parent),
      boardModel(boardModel),
      wonders(wonders),
      wonderDescriptionType(wonderDescriptionType)
{
    foreach(WonderModel::Wonder wonder, this->wonders.keys())
    {
        if(!this->wonderDescriptions.keys().contains(wonder))
        {
            WonderDescription *wonderDescription = new WonderDescription(this->boardModel, wonder, this->wonders[wonder], this->wonderDescriptionType, this);
            this->wonderDescriptions.insert(wonder, wonderDescription);
            connect(wonderDescription, SIGNAL(wonderBuilt(WonderModel::Wonder)), this, SLOT(triggerWonderBuilt(WonderModel::Wonder)));

            if(wonderDescriptionType == WonderDescription::SELECTION)
            {
                this->wonderSelection.insert(wonder, 0);
                connect(wonderDescription, SIGNAL(selectionCountChanged(WonderModel::Wonder, int)), this, SLOT(selectionCountChanged(WonderModel::Wonder, int)));
            }
        }
    }

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setColumnCount(12);

    this->setCompactColumns();

    this->horizontalHeader()->setSortIndicatorShown(true);
    this->setSortingEnabled(true);

    this->setRowCount(this->wonders.count());

    //QColor blue(0x17,0x57,0xe8);
    QColor red(0xea,0x1a,0x1a);
    QColor green(0x16,0xb7,0x16);

    for(int i = 0; i < this->wonders.count(); ++i)
    {
        WonderModel::Wonder wonder = this->wonders.keys()[i];
        const WonderModel *wonderModel = this->boardModel->refWonderModel(wonder);
        QTableWidgetItem *wonderCount;

        if(this->wonderDescriptionType == WonderDescription::SELECTION)
        {
            wonderCount = new SelectionTableItem(0, this->wonders[wonder]);
        }
        else
        {
            wonderCount = new IntegerTableItem(this->wonders[wonder]);
        }
        QTableWidgetItem *nameItem = new QTableWidgetItem(wonderModel->getName());
        IntegerTableItem *tribeCostItem = new IntegerTableItem(wonderModel->getTribesCost());
        IntegerTableItem *goldCostItem = new IntegerTableItem(wonderModel->getGoldCost());
        QTableWidgetItem *woodItem = new QTableWidgetItem(wonderModel->getRequiresWood() ? "Yes" : "No");
        QTableWidgetItem *stoneItem = new QTableWidgetItem(wonderModel->getRequiresStone() ? "Yes" : "No");
        QTableWidgetItem *foodItem = new QTableWidgetItem(wonderModel->getRequiresFood() ? "Yes" : "No");
        QTableWidgetItem *advancePrequisitesItem = new QTableWidgetItem("None");
        QTableWidgetItem *otherRequirementsItem = new QTableWidgetItem(!wonderModel->getOtherRequirements().isEmpty() ? "Yes" : "No");

        this->wonderTableNameItems.insert(wonder, nameItem);
        this->wonderTableCountItems.insert(wonder, wonderCount);

        if(wonderModel->advancePrequisitesMet(this->boardModel->getAdvancesAquired()))
        {
            if(wonderModel->getAdvancePrequisites().count() == 0)
            {
                advancePrequisitesItem->setText("None");
            }
            else
            {
                advancePrequisitesItem->setText("Satisfied");
            }
        }
        else
        {
            advancePrequisitesItem->setText("Incomplete");
        }

        RegionModel *activeRegion = this->boardModel->refActiveRegion();

        if(this->boardModel->isBuildingWonders())
        {
            if(this->boardModel->canBuildWonder(wonder))
            {
                wonderCount->setBackgroundColor(green);
            }

            int tribesCost = wonderModel->getTribesCost();

            if(tribesCost <= activeRegion->getTribes() && tribesCost < this->boardModel->getTribeCount())
            {
                tribeCostItem->setBackgroundColor(green);
            }
            else
            {
                tribeCostItem->setBackgroundColor(red);
            }

            if(wonderModel->getGoldCost() <= this->boardModel->getGold())
            {
                goldCostItem->setBackgroundColor(green);
            }
            else
            {
                goldCostItem->setBackgroundColor(red);
            }

            if(!wonderModel->getRequiresWood() || activeRegion->hasForest())
            {
                woodItem->setBackgroundColor(green);
            }
            else
            {
                woodItem->setBackgroundColor(red);
            }

            if(!wonderModel->getRequiresStone() || activeRegion->hasMountain() || activeRegion->hasVolcano())
            {
                stoneItem->setBackgroundColor(green);
            }
            else
            {
                stoneItem->setBackgroundColor(red);
            }

            if(!wonderModel->getRequiresFood() || activeRegion->hasFarm())
            {
                foodItem->setBackgroundColor(green);
            }
            else
            {
                foodItem->setBackgroundColor(red);
            }

            if(wonderModel->advancePrequisitesMet(this->boardModel->getAdvancesAquired()))
            {
                advancePrequisitesItem->setBackgroundColor(green);
            }
            else
            {
                advancePrequisitesItem->setBackgroundColor(red);
            }

            if(this->boardModel->otherWonderRequirementsMet(wonder))
            {
                otherRequirementsItem->setBackgroundColor(green);
            }
            else
            {
                otherRequirementsItem->setBackgroundColor(red);
            }
        }

        this->setItem(i, 0, wonderCount);
        this->setItem(i, 1, nameItem);
        this->setItem(i, 2, new IntegerTableItem(wonderModel->getVictoryPoints()));
        this->setItem(i, 3, tribeCostItem);
        this->setItem(i, 4, goldCostItem);
        this->setItem(i, 5, woodItem);
        this->setItem(i, 6, stoneItem);
        this->setItem(i, 7, foodItem);
        this->setItem(i, 8, advancePrequisitesItem);
        this->setItem(i, 9, otherRequirementsItem);
        this->setItem(i, 10, new IntegerTableItem(wonderModel->getPositiveEffects().count()));
        this->setItem(i, 11, new IntegerTableItem(wonderModel->getNegativeEffects().count()));
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
    this->resizeColumnToContents(11);

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectWonder()));
}

void WondersTable::setCompactColumns()
{
    QStringList headers;

    QString builtColumnName = "#";

    headers << builtColumnName << "Name" << "VP" << "T" << "G" << "F" << "M/V" << "F" << "Adv." << "Req." << "P" << "N";
    this->setHorizontalHeaderLabels(headers);
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
    this->resizeColumnToContents(11);
}

void WondersTable::setFullColumns()
{
    QStringList headers;
    QString builtColumnName = "Built Total";

    if(this->wonderDescriptionType == WonderDescription::SELECTION)
    {
        builtColumnName = "Wonders Selected";
    }

    headers << builtColumnName << "Name" << "Victory Points" << "Tribes" << "Gold" << "Forest" << "Mountain/Volcano" << "Farm" << "Advance Prequisites" << "Other Requirements" << "Positives" << "Negatives";
    this->setHorizontalHeaderLabels(headers);
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
    this->resizeColumnToContents(11);
}

QMap<WonderModel::Wonder, int> WondersTable::getWonders() const
{
    return this->wonders;
}

QMap<WonderModel::Wonder, int> WondersTable::getSelectedWonders() const
{
    return this->wonderSelection;
}

int WondersTable::getSelectionTotal() const
{
    return this->selectionTotal;
}

void WondersTable::setSelectionTotal(int selectionTotal)
{
    this->selectionTotal = selectionTotal;
    return;
}

void WondersTable::wonderSelectionCountChanged(WonderModel::Wonder wonder, int wonderSelection)
{
    this->wonderSelection[wonder] = wonderSelection;
    QTableWidgetItem *item = this->wonderTableCountItems[wonder];
    SelectionTableItem *selectionItem = dynamic_cast<SelectionTableItem *>(item);

    if(selectionItem != NULL)
    {
        selectionItem->setSelectedCount(wonderSelection);
    }

    emit this->wonderSelectionChanged(wonder, wonderSelection);
}

void WondersTable::selectWonder()
{
    if(this->selectedItems().isEmpty() ||
       !this->item(this->currentRow(), 1))
    {
        return;
    }

    QTableWidgetItem *item = this->item(this->currentRow(), 1);
    WonderModel::Wonder wonder = this->wonderTableNameItems.key(item);
    this->wonderDescriptions[wonder]->show();

    return;
}

void WondersTable::triggerWonderBuilt(WonderModel::Wonder wonder)
{
    this->boardModel->doBuildWonder(wonder);
    emit this->closeTable();
}
