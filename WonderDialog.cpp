#include "WonderDialog.hpp"

#include <QDesktopWidget>
#include <QApplication>

WonderDialog::WonderDialog(BoardModel *boardModel, WonderDescription::WonderDescriptionType wonder, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      isCompact(true),
      WONDERS_DIALOG_SIZE_COMPACT(772,373),
      WONDERS_DIALOG_SIZE_FULL(100, 60),
      BUTTON_TEXT_COMPACT("COMPCAT VIEW"),
      BUTTON_TEXT_FULL("FULL VIEW")
{
    this->setWindowTitle("Wonders (Overview)");
    this->wonderLayout = new QGridLayout(this);
    this->wondersTable = new WondersTable(this->boardModel, this->boardModel->getAllWonders(), wonder, this);
    this->resizeButton = new QPushButton(this->BUTTON_TEXT_COMPACT, this);

    connect(this->resizeButton, SIGNAL(clicked()), this, SLOT(toggleSize()));

    this->setCompactSize();

    this->setLayout(this->wonderLayout);
    this->wonderLayout->addWidget(this->wondersTable, 0, 0);
    this->wonderLayout->addWidget(this->resizeButton, 1, 0);
}

void WonderDialog::setCompactSize()
{
    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    this->resizeButton->setText(this->BUTTON_TEXT_COMPACT);
    this->isCompact = true;
    this->wondersTable->setCompactColumns();
    this->setFixedSize(this->WONDERS_DIALOG_SIZE_COMPACT.width(),this->WONDERS_DIALOG_SIZE_COMPACT.height());
    this->move(mainScreenSize.center() - this->rect().center());
    return;
}

void WonderDialog::setFullSize()
{
    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    this->resizeButton->setText(this->BUTTON_TEXT_FULL);
    this->isCompact = false;
    this->wondersTable->setFullColumns();
    this->setFixedSize(mainScreenSize.width() - this->WONDERS_DIALOG_SIZE_FULL.width(), mainScreenSize.height() - this->WONDERS_DIALOG_SIZE_FULL.height());
    this->move(mainScreenSize.center() - this->rect().center());
    return;
}

void WonderDialog::toggleSize()
{
    if(this->isCompact)
    {
        this->setFullSize();
    }
    else
    {
        this->setCompactSize();
    }
}
