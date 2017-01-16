#ifndef ADVANCESTABLE_HPP
#define ADVANCESTABLE_HPP

#include <QTableWidget>
#include <QMap>

#include "BoardModel.hpp"
#include "AdvanceItem.hpp"

class IntegerTableItem : public QTableWidgetItem
{
public:
    IntegerTableItem(int integer)
        : QTableWidgetItem(QString::number(integer))
    {}

    bool operator< (const QTableWidgetItem &other) const
    {
        return text().toInt() < other.text().toInt();
    }
};

class AdvancesTable : public QTableWidget
{
    Q_OBJECT

private:
    BoardModel *boardModel;

    const QMap<AdvanceModel::Advance, AdvanceItem *>  &advanceItemMap;
    QMap<AdvanceModel::Advance, QTableWidgetItem *> advanceTableItems;

public:
    AdvancesTable(BoardModel *boardModel, const QMap<AdvanceModel::Advance, AdvanceItem *> &advanceItemMap, QWidget *parent = 0);

private slots:
    void selectAdvance();
};


#endif // ADVANCETABLE_HPP
