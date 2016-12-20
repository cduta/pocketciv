#ifndef DIPLOMATICOFFERDIALOG_H
#define DIPLOMATICOFFERDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

#include "BoardModel.hpp"

class DiplomaticOfferDialog : public QDialog
{
    Q_OBJECT

private:
    BoardModel *boardModel;
    QLabel *tribeOfferLabel;
    int goldOffer;
    bool selectingTribes;

public:
    DiplomaticOfferDialog(BoardModel *boardModel,
                  QWidget *parent = 0)
        : QDialog(parent),
          boardModel(boardModel),
          tribeOfferLabel(NULL),
          goldOffer(0),
          selectingTribes(false)
    {
        QWidget *diplomaticOfferWidget = new QWidget(this);
        QGridLayout *diplomaticOfferLayout = new QGridLayout(diplomaticOfferWidget);

        this->tribeOfferLabel = new QLabel();
        this->updateTribeOfferLabel();
        diplomaticOfferLayout->addWidget(new QLabel(QString("Tribe offer: ")), 0,0);
        diplomaticOfferLayout->addWidget(this->tribeOfferLabel,0,1,1,2, Qt::AlignCenter);

        diplomaticOfferLayout->addWidget(new QLabel("Gold offer: ", diplomaticOfferWidget), 1,0);
        QSpinBox *spinBox = new QSpinBox(diplomaticOfferWidget);
        spinBox->setRange(0,this->boardModel->getGold());
        spinBox->setValue(0);
        connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(setGoldOffer(int)));
        diplomaticOfferLayout->addWidget(spinBox,1,1);
        diplomaticOfferLayout->addWidget(new QLabel("/" + QString::number(this->boardModel->getGold()), diplomaticOfferWidget),1,2);

        diplomaticOfferWidget->setLayout(diplomaticOfferLayout);

        QWidget *buttonsWidget = new QWidget(this);
        QGridLayout *buttonsLayout = new QGridLayout(buttonsWidget);

        QPushButton *acceptButton = new QPushButton("Offer", buttonsWidget);
        QPushButton *selectTribesButton = new QPushButton("Select Tribes...", buttonsWidget);
        QPushButton *cancelButton = new QPushButton("Cancel", buttonsWidget);

        connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
        connect(this, SIGNAL(rejected()), this, SLOT(rejectOffer()));
        connect(selectTribesButton, SIGNAL(clicked()), this, SLOT(setSelectTribes()));
        connect(selectTribesButton, SIGNAL(clicked()), this, SLOT(accept()));

        buttonsLayout->addWidget(acceptButton, 0,0);
        buttonsLayout->addWidget(selectTribesButton, 0,1);
        buttonsLayout->addWidget(cancelButton, 0,2);

        buttonsWidget->setLayout(buttonsLayout);

        QGridLayout *layout = new QGridLayout(this);

        layout->addWidget(diplomaticOfferWidget, 0,0);
        layout->addWidget(buttonsWidget, 1,0);

        this->setLayout(layout);
        this->setWindowTitle("Select the diplomatic offer...");
    }

    void decimateOffer()
    {
        this->boardModel->decimateAllSelectedTribes();
        this->boardModel->setGold(this->boardModel->getGold() - this->goldOffer);
        return;
    }

    int getDiplomaticOffer() const
    {
        return this->boardModel->getAllSelectedTribes() + this->goldOffer;
    }

    bool isSelectingTribes() const
    {
        return this->selectingTribes;
    }

    void unsetSelectedTrbes()
    {
        this->setSelectTribes(false);
    }

public slots:
    void updateTribeOfferLabel()
    {
        this->tribeOfferLabel->setText(QString::number(this->boardModel->getAllSelectedTribes()) + "/" + QString::number(this->boardModel->getTribeCount()));
        return;
    }

private slots:
    void setGoldOffer(int howMuchGold)
    {
        this->goldOffer = howMuchGold;
        return;
    }

    void rejectOffer()
    {
        this->boardModel->unselectAllSelectedTribes();
        this->updateTribeOfferLabel();
        this->goldOffer = 0;
        return;
    }

    void setSelectTribes(bool enable = true)
    {
        this->selectingTribes = enable;
    }
};

#endif // DIPLOMATICOFFERDIALOG_H
