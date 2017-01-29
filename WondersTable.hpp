#ifndef WONDERSTABLE_HPP
#define WONDERSTABLE_HPP

#include <QTableWidget>

#include "BoardModel.hpp"
#include "WonderDescription.hpp"
#include "IntegerTableItem.hpp"

class WondersTable : public QTableWidget
{
    Q_OBJECT

    BoardModel *boardModel;
    QList<WonderModel::Wonder> wonders;
    WonderDescription::WonderDescriptionType wonderDescriptionType;

    QMap<WonderModel::Wonder, QTableWidgetItem *> wonderTableItems;

public:
    WondersTable(BoardModel *boardModel,
                 QList<WonderModel::Wonder> wonders,
                 WonderDescription::WonderDescriptionType wonderDescriptionType = WonderDescription::OVERVIEW,
                 QWidget *parent = 0);

    void setCompactColumns();
    void setFullColumns();

private slots:
    void selectWonder();
};

#endif // WONDERSTABLE_HPP
