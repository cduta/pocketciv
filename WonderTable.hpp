#ifndef WONDERTABLE_HPP
#define WONDERTABLE_HPP

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

class WonderTable : public QTableWidget
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
    WonderTable(BoardModel *boardModel,
                 QMap<WonderModel::Wonder, int> wonders,
                 WonderDescription::WonderDescriptionType wonderDescriptionType = WonderDescription::OVERVIEW,
                 QWidget *parent = 0);

    void setCompactColumns();
    void setFullColumns();

    QMap<WonderModel::Wonder, int> getWonders() const;
    QMap<WonderModel::Wonder, int> getSelectedWonders() const;
    int getSelectionTotal() const;

    void setSelectionTotal(int selectionTotal);

    virtual void update();

private slots:
    void wonderSelectionCountChanged(WonderModel::Wonder wonder, int wonderSelection);
    void selectWonder();
    void triggerWonderBuilt(WonderModel::Wonder wonder);
};

#endif // WONDERTABLE_HPP
