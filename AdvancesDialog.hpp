#ifndef ADVANCESDIALOG_H
#define ADVANCESDIALOG_H

#include <QDialog>

#include "BoardModel.hpp"

class AdvancesDialog : public QDialog
{
    Q_OBJECT

public:
    enum AdvanceDialogType {OVERVIEW = 0, AQUIRE_ADVANCE, SELECT_ADVANCES};

signals:
    void advanceIDLeftClicked(int id);
    void advanceIDRightClicked(int id);

private:
    BoardModel *boardModel;
    int activeRegion;
    AdvanceDialogType dialogType;

public:
    // Constructor for the other types of advance dialog.
    AdvancesDialog(BoardModel *boardModel, AdvanceDialogType dialogType = OVERVIEW, QWidget *parent = 0);
    // Constructor to aquire advances.
    AdvancesDialog(BoardModel *boardModel, int activeRegion, QWidget *parent = 0);

private:
    void init();

public:
    // Get-Methods
    AdvanceDialogType getAdvanceDialogType() const;

public slots:
    void updateDialog();
};

#endif // ADVANCESDIALOG_H
