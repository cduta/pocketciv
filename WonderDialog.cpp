#include "WonderDialog.hpp"

#include <QDesktopWidget>
#include <QApplication>

WonderDialog::WonderDialog(BoardModel *boardModel, WonderDescription::WonderDescriptionType wonderDescriptionType, int region, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      isCompact(true),
      WONDERS_DIALOG_SIZE_COMPACT(789,373),
      WONDERS_DIALOG_SIZE_FULL(100, 60),
      BUTTON_TEXT_COMPACT("FULL VIEW"),
      BUTTON_TEXT_FULL("COMPCAT VIEW")
{    
    if(region == -1)
    {
        wonderDescriptionType = WonderDescription::OVERVIEW;
    }

    QString windowTitleText = "UNKNOWN";
    switch(wonderDescriptionType)
    {
        case WonderDescription::OVERVIEW: windowTitleText = "Overview"; break;
        case WonderDescription::REGION_OVERVIEW: windowTitleText = QString("Region %1").arg(region); break;
        case WonderDescription::BUILD: windowTitleText = QString("Build Wonder in Region %1").arg(this->boardModel->refActiveRegion()->getRegion()); break;
        case WonderDescription::SELECTION: windowTitleText = "Selection"; break;
    }

    this->setWindowTitle(QString("Wonders (%1)").arg(windowTitleText));
    this->wonderLayout = new QGridLayout(this);

    QMap<WonderModel::Wonder, int> wonders;

    if(wonderDescriptionType == WonderDescription::REGION_OVERVIEW)
    {
        wonders = this->boardModel->refRegionModel(region)->getBuiltWonders();
    }
    else
    {
        wonders = this->boardModel->getAllWonders();
    }

    this->wondersTable = new WondersTable(this->boardModel, wonders, wonderDescriptionType, this);
    connect(this->wondersTable, SIGNAL(closeTable()), this, SLOT(accept()));

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
