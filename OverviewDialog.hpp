#ifndef OVERVIEWDIALOG_H
#define OVERVIEWDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

#include "BoardModel.hpp"
#include "AdvancesDialog.hpp"

class OverviewDialog : public QDialog
{
    Q_OBJECT

    BoardModel *    boardModel;

    QGridLayout *   layout;

    QPushButton *   showAdvances;
    QPushButton *   showGlory;

    AdvancesDialog *advancesDialog;

public:
    OverviewDialog(BoardModel *boardModel, QWidget *parent = 0);
    ~OverviewDialog();

private slots:
    void showAdvancesTriggered();
    void showGloryTriggered();
};

#endif // OVERVIEWDIALOG_H
