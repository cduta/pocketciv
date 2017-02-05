#ifndef ADVANCESTABLE_HPP
#define ADVANCESTABLE_HPP

#include <QTableWidget>
#include <QMap>

#include "BoardModel.hpp"
#include "AdvanceItem.hpp"
#include "IntegerTableItem.hpp"

class AdvancesTable : public QTableWidget
{
    Q_OBJECT

private:
    BoardModel *boardModel;

    const QMap<AdvanceModel::Advance, AdvanceItem *>  &advanceItemMap;
    QMap<AdvanceModel::Advance, QTableWidgetItem *> advanceTableItems;

public:
    AdvancesTable(BoardModel *boardModel, const QMap<AdvanceModel::Advance, AdvanceItem *> &advanceItemMap, QWidget *parent = 0);

    virtual void update();

private slots:
    void selectAdvance();
};


#endif // ADVANCETABLE_HPP
