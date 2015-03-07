#include "OverviewDialog.hpp"

OverviewDialog::OverviewDialog(BoardModel *boardModel, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      layout(new QGridLayout(this)),
      showAdvances(new QPushButton("Show Advances...", this)),
      showGlory(new QPushButton("Show Glory...", this))
{
    this->setWindowTitle("Overview");
    this->layout->addWidget(this->showAdvances,0,0);
    connect(this->showAdvances, SIGNAL(clicked()), this, SLOT(showAdvancesTriggered()));
    this->layout->addWidget(this->showGlory,1,0);
    connect(this->showGlory, SIGNAL(clicked()), this, SLOT(showGloryTriggered()));

    this->advancesDialog = new AdvancesDialog(this->boardModel, AdvanceItem::OVERVIEW, this);

    this->setLayout(this->layout);
}

OverviewDialog::~OverviewDialog()
{
    this->layout->deleteLater();
}

void OverviewDialog::showAdvancesTriggered()
{
    this->advancesDialog->show();
    return;
}

void OverviewDialog::showGloryTriggered()
{
    return;
}

