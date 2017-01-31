#include "WonderDescription.hpp"

#include <QScrollBar>
#include <QLabel>

WonderDescription::WonderDescription(BoardModel *boardModel, WonderModel::Wonder wonder, int wonderCount, WonderDescription::WonderDescriptionType wonderDescriptionType, QObject *parent)
    : QObject(parent),
      boardModel(boardModel),
      wonder(wonder),
      wonderSelection(0),
      wonderCount(wonderCount),
      wonderDescriptionType(wonderDescriptionType)
{
    this->layout = new QGridLayout(&this->dialog);
    this->dialog.setLayout(this->layout);

    this->buildWonder = new QPushButton(QString("Build"), &this->dialog);

    this->wonderSelectionSpinBox = new QSpinBox(&this->dialog);
    this->wonderSelectionSpinBox->setRange(0,this->wonderCount);
    this->wonderSelectionSpinBox->setValue(0);

    QLabel *spinBoxLabel = new QLabel(QString("/%1 selected.").arg(this->wonderCount), &this->dialog);

    this->submitSelection = new QPushButton(QString("Submit"), &this->dialog);
    this->cancel = new QPushButton(QString("Cancel"), &this->dialog);

    QFont font("monospace");
    this->description = new QPlainTextEdit(&this->dialog);
    this->description->setFont(font);
    this->description->setReadOnly(true);

    this->layout->addWidget(this->description, 0,0,1,4);
    if(this->wonderDescriptionType == WonderDescription::BUILD)
    {
        this->layout->addWidget(this->buildWonder, 1,0,1,4);

        this->buildWonder->setEnabled(this->boardModel->canBuildWonder(wonder));
        spinBoxLabel->setVisible(false);
        this->wonderSelectionSpinBox->setVisible(false);
        this->submitSelection->setVisible(false);
        this->cancel->setVisible(false);

        connect(this->buildWonder, SIGNAL(clicked()), this, SLOT(buildWonderClicked()));
        connect(this->buildWonder, SIGNAL(clicked()), &this->dialog, SLOT(close()));
    }
    else if(this->wonderDescriptionType == WonderDescription::SELECTION)
    {
        this->buildWonder->setVisible(false);
        this->layout->addWidget(this->wonderSelectionSpinBox, 1,0);
        this->layout->addWidget(spinBoxLabel, 1,1);
        this->layout->addWidget(this->submitSelection, 1,2);
        this->layout->addWidget(this->cancel, 1,3);

        connect(this->submitSelection, SIGNAL(clicked()), this, SLOT(wonderSelectionChanged()));
        connect(this->cancel, SIGNAL(clicked()), &this->dialog, SLOT(close()));
    }
    else
    {
        spinBoxLabel->setVisible(false);
        this->wonderSelectionSpinBox->setVisible(false);
        this->submitSelection->setVisible(false);
        this->cancel->setVisible(false);
        this->buildWonder->setVisible(false);
    }

    this->updateDescription();

    this->dialog.resize(400,400);
}


void WonderDescription::updateDescription()
{
    const WonderModel *wonderModel = this->boardModel->refWonderModel(this->wonder);
    this->dialog.setWindowTitle(QString("%1 (%2 VP)").arg(wonderModel->getName()).arg(QString::number(wonderModel->getVictoryPoints())));

    QString wood;

    if(wonderModel->getRequiresWood())
    {
        wood = "Yes";

        if(this->wonderDescriptionType == WonderDescription::BUILD)
        {
            if(this->boardModel->refActiveRegion()->hasForest())
            {
                wood.append(" (OK)");
            }
            else
            {
                wood.append(" (  )");
            }
        }
    }
    else
    {
        wood = "No";
    }

    QString stone;

    if(wonderModel->getRequiresStone())
    {
        stone = "Yes";

        if(this->wonderDescriptionType == WonderDescription::BUILD)
        {
            if(this->boardModel->refActiveRegion()->hasMountain() || this->boardModel->refActiveRegion()->hasVolcano())
            {
                stone.append(" (OK)");
            }
            else
            {
                stone.append(" (  )");
            }
        }
    }
    else
    {
        stone = "No";
    }

    QString food;

    if(wonderModel->getRequiresFood())
    {
        food = "Yes";

        if(this->wonderDescriptionType == WonderDescription::BUILD)
        {
            if(this->boardModel->refActiveRegion()->hasFarm())
            {
                food.append(" (OK)");
            }
            else
            {
                food.append(" (  )");
            }
        }
    }
    else
    {
        food = "No";
    }

    int tribesCost = wonderModel->getTribesCost();
    QString tcString = QString::number(tribesCost);
    QString gcString = QString::number(wonderModel->getGoldCost());

    if(this->wonderDescriptionType == WonderDescription::BUILD)
    {
        tcString.append(QString(" / %1").arg(this->boardModel->refActiveRegion()->getTribes()));
        gcString.append(QString(" / %1").arg(this->boardModel->getGold()));
    }

    this->description->clear();
    //this->description->insertPlainText(QString("Victory Points                    : %1\n\n").arg(wonderModel->getVictoryPoints()));
    this->description->insertPlainText(QString("Tribes Cost                       : %1\n").arg(tcString));
    this->description->insertPlainText(QString("Gold Cost                         : %1\n").arg(gcString));
    this->description->insertPlainText(QString("Wood (Forest) required            : %1\n").arg(wood));
    this->description->insertPlainText(QString("Stone (Mountain/Volcano) required : %1\n").arg(stone));
    this->description->insertPlainText(QString("Food (Farm) required              : %1\n").arg(food));

    QList<AdvanceModel::Advance> advances = wonderModel->getAdvancePrequisites();

    if(!advances.isEmpty())
    {
        this->description->insertPlainText(QString("\nAdvance prequisite\n"));

        QString advancesListString = "None";

        if(advances.count() > 1)
        {
            advancesListString = "";
            for(int i = 0; i < advances.count() - 2; ++i)
            {
                advancesListString = advancesListString.append(QString("%1, ").arg(this->boardModel->refAdvanceModel(advances[i])->getName()));
            }

            advancesListString = advancesListString.append(QString("%1 and %2")
                                               .arg(this->boardModel->refAdvanceModel(advances[advances.count() - 2])->getName())
                                               .arg(this->boardModel->refAdvanceModel(advances[advances.count() - 1])->getName()));
        }
        else if(advances.count() == 1)
        {
            advancesListString = QString("%1").arg(this->boardModel->refAdvanceModel(advances.first())->getName());
        }

        this->description->insertPlainText(QString(" %1").arg(advancesListString));
    }

    if(!wonderModel->getOtherRequirements().isEmpty())
    {
        this->description->insertPlainText(QString("\nOther requirements\n"));
        foreach(QString requirement, wonderModel->getOtherRequirements())
        {
            this->description->insertPlainText(QString("- %1").arg(requirement));
        }
    }

    if(!wonderModel->getEffects().isEmpty())
    {
        this->description->insertPlainText(QString("\nEffects\n%1").arg(wonderModel->getEffects()));
    }

    return;
}


void WonderDescription::show()
{
    this->wonderSelectionSpinBox->setValue(this->wonderSelection);
    this->dialog.show();
    this->description->verticalScrollBar()->setValue(0);
    return;
}

void WonderDescription::buildWonderClicked()
{
    emit this->wonderBuilt(this->wonder);
    return;
}

void WonderDescription::wonderSelectionChanged()
{
    this->wonderSelection = this->wonderSelectionSpinBox->value();
    emit this->selectionCountChanged(this->wonder, this->wonderSelection);
    this->dialog.close();
}
