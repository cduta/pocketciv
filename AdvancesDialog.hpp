#ifndef ADVANCESDIALOG_H
#define ADVANCESDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "BoardModel.hpp"
#include "AdvanceItem.hpp"

class AdvancesDialog : public QDialog
{
    Q_OBJECT

public:
    enum AdvanceDialogType {OVERVIEW = 0, AQUIRE_ADVANCE, SELECT_ADVANCES};

signals:
    void advanceIDLeftClicked(int id);
    void advanceIDRightClicked(int id);
    void selectionLimitReached(int lastSelectedID);

private:
    BoardModel *        boardModel;
    int                 activeRegion;
    AdvanceDialogType   dialogType;
    int                 selectionLimit;

    QGridLayout *       layout;
    QGraphicsView *     graphicsView;
    QGraphicsScene *    graphicsScene;

    QMap<AdvanceModel::Advance, AdvanceItem *>  advanceItemMap;
    QGraphicsPixmapItem *                       advanceBackground;
    QGraphicsPixmapItem *                       advanceTitle;

public:
    // Constructor for the other types of advance dialog. Cannot be AQUIRE_ADVANCE since it needs an active region specified.
    AdvancesDialog(BoardModel *boardModel, AdvanceDialogType dialogType = OVERVIEW, QWidget *parent = 0);
    // Constructor to aquire advances.
    AdvancesDialog(BoardModel *boardModel, int activeRegion, QWidget *parent = 0);
    ~AdvancesDialog();

private:
    void init();

public:
    // Set-Methods
    void setSelectionLimit(int selectionLimit);

    // Get-Methods
    AdvanceDialogType getAdvanceDialogType() const;

public slots:
    void updateDialog();
};

#endif // ADVANCESDIALOG_H
