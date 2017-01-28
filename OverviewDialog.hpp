#ifndef OVERVIEWDIALOG_H
#define OVERVIEWDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

#include "BoardModel.hpp"
#include "AdvancesDialog.hpp"
#include "WondersTable.hpp"

class OverviewDialog : public QDialog
{
    Q_OBJECT

    BoardModel *    boardModel;

    QGridLayout *   layout;

    QPushButton *   showCards;
    QPushButton *   showAdvances;
    QPushButton *   showWonders;
    QPushButton *   showGlory;

    AdvancesDialog *advancesDialog;
    WondersTable * wondersTable;

public:
    OverviewDialog(BoardModel *boardModel, QWidget *parent = 0);
    ~OverviewDialog();

private slots:
    void showCardsTriggered();
    void showAdvancesTriggered();
    void showWondersTriggered();
    void showGloryTriggered();
};

#endif // OVERVIEWDIALOG_H
