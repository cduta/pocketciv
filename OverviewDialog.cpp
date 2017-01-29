#include "OverviewDialog.hpp"

#include "WonderDescription.hpp"

OverviewDialog::OverviewDialog(BoardModel *boardModel, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      layout(new QGridLayout(this)),
      showCards(new QPushButton("Show Cards...", this)),
      showAdvances(new QPushButton("Show Advances...", this)),
      showWonders(new QPushButton("Show Wonders...", this)),
      showGlory(new QPushButton("Show Glory...", this))
{
    this->setWindowTitle("Overview");
    this->layout->addWidget(this->showCards,0,0);
    connect(this->showCards, SIGNAL(clicked()), this, SLOT(showCardsTriggered()));
    connect(this->showCards, SIGNAL(clicked()), this, SLOT(close()));
    this->layout->addWidget(this->showAdvances,1,0);
    connect(this->showAdvances, SIGNAL(clicked()), this, SLOT(showAdvancesTriggered()));
    connect(this->showAdvances, SIGNAL(clicked()), this, SLOT(close()));
    this->layout->addWidget(this->showWonders,2,0);
    connect(this->showWonders, SIGNAL(clicked()), this, SLOT(showWondersTriggered()));
    connect(this->showWonders, SIGNAL(clicked()), this, SLOT(close()));
    this->layout->addWidget(this->showGlory,3,0);
    connect(this->showGlory, SIGNAL(clicked()), this, SLOT(showGloryTriggered()));
    connect(this->showGlory, SIGNAL(clicked()), this, SLOT(close()));

    this->advancesDialog = new AdvancesDialog(this->boardModel, AdvanceItem::OVERVIEW, this);
    this->wonderDialog = new WonderDialog(this->boardModel, WonderDescription::OVERVIEW, this);

    this->setLayout(this->layout);
}

OverviewDialog::~OverviewDialog()
{
    this->layout->deleteLater();
}

void OverviewDialog::showCardsTriggered()
{
    return;
}

void OverviewDialog::showAdvancesTriggered()
{
    this->advancesDialog->show();
    return;
}

void OverviewDialog::showWondersTriggered()
{
    this->wonderDialog->show();
    return;
}

void OverviewDialog::showGloryTriggered()
{
    return;
}
