#include "AdvancesDialog.hpp"

AdvancesDialog::AdvancesDialog(BoardModel *boardModel, AdvanceDialogType dialogType, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      activeRegion(-1),
      dialogType(dialogType)
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

