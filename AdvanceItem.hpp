#ifndef ADVANCEITEM_H
#define ADVANCEITEM_H


#include <QDialog>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include "BoardModel.hpp"
#include "AdvanceModel.hpp"

class DescriptionDialog : public QObject
{
    Q_OBJECT

signals:
    void advanceAquired(AdvanceModel::Advance advance);

public:
    BoardModel *boardModel;
    AdvanceModel::Advance advance;
    QDialog dialog;
    QGridLayout *layout;
    QPlainTextEdit *description;
    QPushButton *aquireAdvance;
    QPushButton *cancel;

public:
    DescriptionDialog()
    {
        this->layout = new QGridLayout(&this->dialog);
        this->dialog.setLayout(this->layout);

        this->aquireAdvance = new QPushButton(QString("Aquire"), &this->dialog);
        this->cancel = new QPushButton(QString("Cancel"), &this->dialog);

        this->description = new QPlainTextEdit(&this->dialog);

        connect(this->aquireAdvance, SIGNAL(clicked()), this, SLOT(aquireAdvanceClicked()));
        connect(this->aquireAdvance, SIGNAL(clicked()), &this->dialog, SLOT(close()));
        connect(this->cancel, SIGNAL(clicked()), &this->dialog, SLOT(close()));
    }

    void show()
    {
        this->dialog.show();
        this->description->verticalScrollBar()->setValue(0);
        return;
    }

private slots:
    void aquireAdvanceClicked()
    {
        this->boardModel->refActiveRegion()->setAdvanceAquired(true);
        emit this->advanceAquired(this->advance);
        return;
    }
};

class AdvanceItem : public QGraphicsPixmapItem
{

public:
    enum AdvanceItemType {OVERVIEW = 0, AQUIRE = 1, SELECTABLE = 2};

private:
    AdvanceModel::Advance advance;
    BoardModel *boardModel;
    AdvanceItemType advanceType;
    DescriptionDialog descriptionDialog;

public:
    AdvanceItem(qreal xPos,
                qreal yPos,
                BoardModel *boardModel,
                AdvanceModel::Advance advance,
                AdvanceItemType advanceType,
                QGraphicsItem *parent = 0);

private:
    void updateDescription();

public:
    void updateAdvanceItem();
    void showDescriptionDialog();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ADVANCEITEM_H
