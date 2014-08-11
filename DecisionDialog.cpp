#include "DecisionDialog.hpp"

#include <QLabel>
#include <QPushButton>

DecisionDialog::DecisionDialog(const QString &title, const QString &text, const QString &acceptButtonText, const QString &rejectButtonText, bool showOverview)
{
    QWidget *howManyWidget = new QWidget(this);
    QGridLayout *howManyLayout = new QGridLayout(howManyWidget);

    int buttonCount = 2;

    if(showOverview)
    {
        buttonCount++;
    }

    howManyLayout->addWidget(new QLabel(text, howManyWidget), 0,0,1,buttonCount);
    howManyWidget->setLayout(howManyLayout);

    QWidget *buttonsWidget = new QWidget(this);
    QGridLayout *buttonsLayout = new QGridLayout(buttonsWidget);

    QPushButton *acceptButton = new QPushButton(acceptButtonText, buttonsWidget);
    QPushButton *rejectButton = new QPushButton(rejectButtonText, buttonsWidget);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(rejectButton, SIGNAL(clicked()), this, SLOT(reject()));

    buttonsLayout->addWidget(acceptButton, 0,0);
    buttonsLayout->addWidget(rejectButton, 0,1);

    if(showOverview)
    {
        QPushButton *overviewButton = new QPushButton("Overview", buttonsWidget);
        connect(overviewButton, SIGNAL(clicked()), this, SLOT(showOverview()));
        buttonsLayout->addWidget(overviewButton, 0,2);
    }

    buttonsWidget->setLayout(buttonsLayout);

    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(howManyWidget, 0,0);
    layout->addWidget(buttonsWidget, 1,0);

    this->setLayout(layout);
    this->setWindowTitle(title);
}
