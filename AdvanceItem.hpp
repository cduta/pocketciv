#ifndef ADVANCEITEM_H
#define ADVANCEITEM_H

#include <QGraphicsPixmapItem>
#include <QDialog>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QGraphicsSceneMouseEvent>

#include "BoardModel.hpp"
#include "AdvanceModel.hpp"

class AdvanceItem : public QGraphicsPixmapItem
{
    AdvanceModel::Advance advance;
    BoardModel *boardModel;
    bool selected;

    QDialog descriptionDialog;
    QGridLayout *layout;
    QPlainTextEdit *description;

public:
    AdvanceItem(qreal xPos,
                qreal yPos,
                BoardModel *boardModel,
                AdvanceModel::Advance advance,
                QGraphicsItem *parent = 0);

private:
    void updateDesription();

public:
    void updateAdvanceItem();

    void toggleSelected();

    // Set-Methods
    void setSelected(bool selected);

    // Get-Methods
    bool isSelected() const;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ADVANCEITEM_H
