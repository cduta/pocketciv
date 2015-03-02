#ifndef ADVANCEITEM_H
#define ADVANCEITEM_H

#include <QGraphicsPixmapItem>

#include "BoardModel.hpp"
#include "AdvanceModel.hpp"

class AdvanceItem : public QGraphicsPixmapItem
{
    AdvanceModel::Advance advance;
    BoardModel *boardModel;
    bool selected;

public:
    AdvanceItem(qreal xPos,
                qreal yPos,
                BoardModel *boardModel,
                AdvanceModel::Advance advance,
                QGraphicsItem *parent = 0);

private:
    void updateAdvanceItem();

public:
    void toggleSelected();

    // Set-Methods
    void setSelected(bool selected);

    // Get-Methods
    bool isSelected() const;
};

#endif // ADVANCEITEM_H
