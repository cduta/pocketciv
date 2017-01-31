#ifndef WONDERSTABLE_HPP
#define WONDERSTABLE_HPP

#include <QTableWidget>

#include "BoardModel.hpp"
#include "WonderDescription.hpp"
#include "IntegerTableItem.hpp"

class SelectionTableItem : public QTableWidgetItem
{
    int selected;
    int total;

public:
    SelectionTableItem(int selected, int total)
        : QTableWidgetItem(QString("%1/%2").arg(selected).arg(total))
    {}

    void setSelectedCount(int selected)
    {
        this->selected = selected;
        this->setText(QString("%1/%2").arg(selected).arg(total));
    }

    bool operator<(const SelectionTableItem &other) const
    {
        return this->selected < other.selected;
    }
};

class WondersTable : public QTableWidget
{
    Q_OBJECT

signals:
    void closeTable();
    void wonderSelectionChanged(WonderModel::Wonder wonder, int wonderSelection);

private:
    BoardModel *boardModel;
    QMap<WonderModel::Wonder, int> wonders;
    QMap<WonderModel::Wonder, int> wonderSelection;
    WonderDescription::WonderDescriptionType wonderDescriptionType;

    QMap<WonderModel::Wonder, QTableWidgetItem *> wonderTableNameItems;
    QMap<WonderModel::Wonder, QTableWidgetItem *> wonderTableCountItems;
    QMap<WonderModel::Wonder, WonderDescription *> wonderDescriptions;

    int selectionTotal;

public:
    WondersTable(BoardModel *boardModel,
                 QMap<WonderModel::Wonder, int> wonders,
                 WonderDescription::WonderDescriptionType wonderDescriptionType = WonderDescription::OVERVIEW,
                 QWidget *parent = 0);

    void setCompactColumns();
    void setFullColumns();

    QMap<WonderModel::Wonder, int> getWonders() const;
    QMap<WonderModel::Wonder, int> getSelectedWonders() const;
    int getSelectionTotal() const;

    void setSelectionTotal(int selectionTotal);

private slots:
    void wonderSelectionCountChanged(WonderModel::Wonder wonder, int wonderSelection);
    void selectWonder();
    void triggerWonderBuilt(WonderModel::Wonder wonder);
};

#endif // WONDERSTABLE_HPP
