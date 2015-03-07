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

signals:
    void advanceIDLeftClicked(int id);
    void advanceIDRightClicked(int id);
    void selectionLimitReached(int lastSelectedID);

private:
    BoardModel *                 boardModel;
    AdvanceItem::AdvanceItemType advanceItemType;
    int                          selectionLimit;

    QGridLayout *         layout;
    QGraphicsView *       graphicsView;
    QGraphicsScene *      graphicsScene;

    QMap<AdvanceModel::Advance, AdvanceItem *>  advanceItemMap;
    QGraphicsPixmapItem *                       advanceBackground;
    QGraphicsPixmapItem *                       advanceTitle;

public:
    AdvancesDialog(BoardModel *boardModel, AdvanceItem::AdvanceItemType advanceItemType = AdvanceItem::OVERVIEW, QWidget *parent = 0);
    ~AdvancesDialog();

    void disconnetAll();

private:
    void init();

public:
    // Set-Methods
    void setSelectionLimit(int selectionLimit);

    // Get-Methods
    AdvanceItem::AdvanceItemType getAdvanceDialogType() const;

public slots:
    void updateDialog();
};

#endif // ADVANCESDIALOG_H
