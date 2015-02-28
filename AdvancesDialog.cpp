#include "AdvancesDialog.hpp"

#include "AdvanceItem.hpp"

AdvancesDialog::AdvancesDialog(BoardModel *boardModel, AdvanceDialogType dialogType, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      activeRegion(-1),
      dialogType(dialogType),
      layout(new QGridLayout(this))
{
    assert(dialogType != AQUIRE_ADVANCE);
    this->init();
}

AdvancesDialog::AdvancesDialog(BoardModel *boardModel, int activeRegion, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      activeRegion(activeRegion),
      dialogType(AQUIRE_ADVANCE)
{
    this->init();
}

void AdvancesDialog::init()
{
    connect(this->boardModel, SIGNAL(boardUpdated()), this, SLOT(updateDialog()));

    this->graphicsScene = new QGraphicsScene(this);
    this->graphicsView = new QGraphicsView(this->graphicsScene, this);
    this->layout->addWidget(this->graphicsView, 0,0);
    this->setLayout(this->layout);
    this->resize(650,450);

    this->graphicsScene->addItem
            (
                new AdvanceItem(10,10,
                                this->boardModel,
                                AdvanceModel::AGRICULTURE)
            );

    this->updateDialog();
    return;
}

AdvancesDialog::AdvanceDialogType AdvancesDialog::getAdvanceDialogType() const
{
    return this->dialogType;
}

void AdvancesDialog::updateDialog()
{
    return;
}

