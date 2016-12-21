#ifndef HOWMANYDIALOG_HPP
#define HOWMANYDIALOG_HPP

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

#include "BoardModel.hpp"

class HowManyDialog : public QDialog
{
    Q_OBJECT

protected:
    BoardModel *boardModel;
    int howMany;

public:
    HowManyDialog(BoardModel *boardModel,
                  const QString &title,
                  const QString &prefixText,
                  const QString &suffixText,
                  int max = 99,
                  const QString &acceptButtonText = QString("Ok"),
                  const QString &rejectButtonText = QString("Cancel"),
                  QWidget *parent = 0)
        : QDialog(parent),
          boardModel(boardModel),
          howMany(1)
    {
        QWidget *howManyWidget = new QWidget(this);
        QGridLayout *howManyLayout = new QGridLayout(howManyWidget);

        howManyLayout->addWidget(new QLabel(prefixText, howManyWidget), 0,0);
        QSpinBox *spinBox = new QSpinBox(howManyWidget);
        spinBox->setRange(0,max);
        spinBox->setValue(1);
        connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(setHowMany(int)));
        howManyLayout->addWidget(spinBox,0,1);
        howManyLayout->addWidget(new QLabel(suffixText, howManyWidget),0,2);

        howManyWidget->setLayout(howManyLayout);

        QWidget *buttonsWidget = new QWidget(this);
        QGridLayout *buttonsLayout = new QGridLayout(buttonsWidget);

        QPushButton *acceptButton = new QPushButton(acceptButtonText, buttonsWidget);
        QPushButton *rejectButton = new QPushButton(rejectButtonText, buttonsWidget);

        connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(rejectButton, SIGNAL(clicked()), this, SLOT(reject()));

        buttonsLayout->addWidget(acceptButton, 0,0);
        buttonsLayout->addWidget(rejectButton, 0,1);

        buttonsWidget->setLayout(buttonsLayout);

        QGridLayout *layout = new QGridLayout(this);

        layout->addWidget(howManyWidget, 0,0);
        layout->addWidget(buttonsWidget, 1,0);

        this->setLayout(layout);
        this->setWindowTitle(title);
    }

public:
    int getHowMany() const
    {
        return this->howMany;
    }

private slots:
    void setHowMany(int howMany)
    {
        this->howMany = howMany;
        return;
    }
};

class MoveTribeDialog : public HowManyDialog
{
    Q_OBJECT

    RegionModel *               from;
    RegionModel *               to;
    BoardModel::MoveTribesType  moveTribesType;

public:
    MoveTribeDialog(BoardModel *boardModel, const QString &suffix, int max, RegionModel *from, RegionModel *to, BoardModel::MoveTribesType moveTribesType, QWidget *parent = 0)
        : HowManyDialog(
              boardModel,
              "Move Tribes...",
              "Move",
              suffix,
              max,
              "Move",
              "Cancel",
              parent),
          from(from),
          to(to),
          moveTribesType(moveTribesType)
    {
        connect(this, SIGNAL(accepted()), this, SLOT(applyMoveTribes()));
        connect(this, SIGNAL(rejected()), this, SLOT(rejectMoveTribes()));
    }

private slots:
    void applyMoveTribes()
    {        
        this->boardModel->moveTribes(this->from->getRegion(),
                                     this->to->getRegion(),
                                     this->howMany,
                                     this->moveTribesType);
        this->boardModel->unselectAllRegions();

        return;
    }

    void rejectMoveTribes()
    {
        this->boardModel->unselectAllRegions();
        return;
    }
};

#endif // HOWMANYDIALOG_HPP
