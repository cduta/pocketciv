#include "WondersTable.hpp"

#include <QAbstractItemView>
#include <QHeaderView>

WondersTable::WondersTable(BoardModel *boardModel, QList<WonderModel::Wonder> wonders, WonderDescription::WonderDescriptionType wonderDescriptionType, QWidget *parent)
    : QTableWidget(parent),
      boardModel(boardModel),
      wonders(wonders),
      wonderDescriptionType(wonderDescriptionType)
{
    foreach(WonderModel::Wonder wonder, this->wonders)
    {
        if(!this->wonderDescriptions.keys().contains(wonder))
        {
            this->wonderDescriptions.insert(wonder, new WonderDescription(this->boardModel, wonder, this->wonderDescriptionType, this));
        }
    }

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setColumnCount(12);

    this->setCompactColumns();

    this->horizontalHeader()->setSortIndicatorShown(true);
    this->setSortingEnabled(true);

    this->setRowCount(this->wonders.count());

    QColor blue(0x17,0x57,0xe8);
    QColor red(0xea,0x1a,0x1a);
    QColor green(0x16,0xb7,0x16);

    for(int i = 0; i < this->wonders.count(); ++i)
    {
        WonderModel::Wonder wonder = this->wonders[i];
        const WonderModel *wonderModel = this->boardModel->refWonderModel(wonder);
        QTableWidgetItem *aquirableItem = new IntegerTableItem(this->boardModel->getAllBuiltWonders()[wonder]);
        QTableWidgetItem *nameItem = new QTableWidgetItem(wonderModel->getName());
        IntegerTableItem *tribeCostItem = new IntegerTableItem(wonderModel->getTribesCost());
        IntegerTableItem *goldCostItem = new IntegerTableItem(wonderModel->getGoldCost());
        QTableWidgetItem *woodItem = new QTableWidgetItem(wonderModel->getRequiresWood() ? "Yes" : "No");
        QTableWidgetItem *stoneItem = new QTableWidgetItem(wonderModel->getRequiresStone() ? "Yes" : "No");
        QTableWidgetItem *foodItem = new QTableWidgetItem(wonderModel->getRequiresFood() ? "Yes" : "No");
        QTableWidgetItem *advancePrequisitesItem = new QTableWidgetItem(!wonderModel->getAdvancePrequisites().isEmpty() ? "Yes" : "No");
        QTableWidgetItem *otherRequirementsItem = new QTableWidgetItem(!wonderModel->getOtherRequirements().isEmpty() ? "Yes" : "No");

        this->wonderTableItems.insert(wonder, nameItem);

        this->setItem(i, 0, aquirableItem);
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

void WondersTable::selectWonder()
{
    if(this->selectedItems().isEmpty() ||
       !this->item(this->currentRow(), 1))
    {
        return;
    }

    QTableWidgetItem *item = this->item(this->currentRow(), 1);
    WonderModel::Wonder wonder = this->wonderTableItems.key(item);
    this->wonderDescriptions[wonder]->show();

    return;
}
