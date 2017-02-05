#include "AdvancesDialog.hpp"

#include <QMessageBox>
#include <QDesktopWidget>

AdvancesDialog::AdvancesDialog(BoardModel *boardModel, AdvanceItem::AdvanceItemType advanceItemType, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      advanceItemType(advanceItemType),
      selectionLimit(1),
      layout(new QGridLayout(this))
{
    assert(this->advanceItemType != AdvanceItem::AQUIRE || this->boardModel->refActiveRegion() != NULL);
    this->init();
}

AdvancesDialog::~AdvancesDialog()
{
    foreach(AdvanceItem *advanceItem, this->advanceItemMap.values())
    {
        delete advanceItem;
    }
    this->advanceItemMap.clear();

    delete this->advanceBackground;
    delete this->advanceTitle;
}

void AdvancesDialog::disconnetAll()
{
    disconnect(this->boardModel, SIGNAL(boardUpdated()), this, SLOT(updateDialog()));
    disconnect(this->boardModel, SIGNAL(goldChanged(int)), this, SLOT(updateDialog()));
    disconnect(this->boardModel, SIGNAL(advanceAquired(AdvanceModel::Advance)), this, SLOT(updateDialog()));
    disconnect(this->boardModel, SIGNAL(changeAdvanceSelected(AdvanceModel::Advance)), this, SLOT(updateDialog()));

    if(this->advanceItemType == AdvanceItem::SELECTABLE)
    {
        disconnect(this->okButton, SIGNAL(clicked()), this, SLOT(selectionDone()));
    }

    return;
}

void AdvancesDialog::init()
{
    switch(this->advanceItemType)
    {
        case AdvanceItem::OVERVIEW: this->setWindowTitle("Advances Overview"); break;
        case AdvanceItem::AQUIRE: this->setWindowTitle("Aquire Advances"); break;
        case AdvanceItem::SELECTABLE:
            this->selectionLimit = boardModel->getTribeCount();
            this->setWindowTitle(QString("Select Advances (0 of %1)").arg(this->selectionLimit));

            break;
    }

    connect(this->boardModel, SIGNAL(boardUpdated()), this, SLOT(updateDialog()));
    connect(this->boardModel, SIGNAL(goldChanged(int)), this, SLOT(updateDialog()));
    connect(this->boardModel, SIGNAL(advanceAquired(AdvanceModel::Advance)), this, SLOT(updateDialog()));
    connect(this->boardModel, SIGNAL(changeAdvanceSelected(AdvanceModel::Advance)), this, SLOT(updateDialog()));

    this->graphicsScene = new QGraphicsScene(this);
    this->graphicsView = new QGraphicsView(this->graphicsScene, this);
    this->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    this->layout->addWidget(this->graphicsView, 0,0, 1,2);

    this->tableButton = new QPushButton("Table View", this);

    this->okButton = NULL;
    if(this->advanceItemType == AdvanceItem::SELECTABLE)
    {
        this->okButton = new QPushButton(QString("Ok"), this);
        this->layout->addWidget(this->okButton, 1,0);
        connect(this->okButton, SIGNAL(clicked()), this, SLOT(selectionDone()));
        this->layout->addWidget(this->tableButton, 1,1);
    }
    else
    {
        this->layout->addWidget(this->tableButton, 1,0, 1,2);
    }

    this->setLayout(this->layout);
    this->resize(650,450);

    this->advanceBackground = new QGraphicsPixmapItem(QPixmap(":/advance_background"));
    this->advanceBackground->setPos(0,0);

    this->advanceTitle = new QGraphicsPixmapItem(QPixmap(":/advance_title"));
    this->advanceTitle->setPos(0,0);

    this->advanceItemMap.insert(AdvanceModel::AGRICULTURE,
                              new AdvanceItem(10+0,63+0,this->boardModel,AdvanceModel::AGRICULTURE, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::HORTICULTURE,
                              new AdvanceItem(10+0,63+25*3,this->boardModel,AdvanceModel::HORTICULTURE, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::SENSE_OF_COMMUNITY,
                              new AdvanceItem(10+0,63+25*8,this->boardModel,AdvanceModel::SENSE_OF_COMMUNITY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MYTHOLOGY,
                              new AdvanceItem(10+0,63+(25*20),this->boardModel,AdvanceModel::MYTHOLOGY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::ASTRONOMY,
                              new AdvanceItem(10+0,63+(25*28),this->boardModel,AdvanceModel::ASTRONOMY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::NAVIGATION,
                              new AdvanceItem(10+0,63+(25*31),this->boardModel,AdvanceModel::NAVIGATION, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::FISHING,
                              new AdvanceItem(10+0,63+(25*34),this->boardModel,AdvanceModel::FISHING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MASONRY,
                              new AdvanceItem(10+0,63+(25*39),this->boardModel,AdvanceModel::MASONRY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MINING,
                              new AdvanceItem(10+0,63+(25*42),this->boardModel,AdvanceModel::MINING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::LITERACY,
                              new AdvanceItem(10+0,63+(25*56),this->boardModel,AdvanceModel::LITERACY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MUSIC,
                              new AdvanceItem(10+0,63+(25*59),this->boardModel,AdvanceModel::MUSIC, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::COINAGE,
                              new AdvanceItem(10+0,63+(25*64),this->boardModel,AdvanceModel::COINAGE, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::CARTAGE,
                              new AdvanceItem(10+0,63+(25*69),this->boardModel,AdvanceModel::CARTAGE, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::CULTURE_OF_THIEVES,
                              new AdvanceItem(10+0,63+(25*74),this->boardModel,AdvanceModel::CULTURE_OF_THIEVES, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::SIMPLE_TOOLS,
                              new AdvanceItem(10+0,63+(25*79),this->boardModel,AdvanceModel::SIMPLE_TOOLS, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::BASIC_TOOLS,
                              new AdvanceItem(10+0,63+(25*84),this->boardModel,AdvanceModel::BASIC_TOOLS, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::SLAVE_LABOR,
                              new AdvanceItem(10+0,63+(25*89),this->boardModel,AdvanceModel::SLAVE_LABOR, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::STORY_TELLING,
                              new AdvanceItem(10+0,63+(25*94),this->boardModel,AdvanceModel::STORY_TELLING, this->advanceItemType));

    this->advanceItemMap.insert(AdvanceModel::IRRIGATION,
                              new AdvanceItem(10+168*2,63+25*0,this->boardModel,AdvanceModel::IRRIGATION, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::EQUESTRIAN,
                              new AdvanceItem(10+168*2,63+25*3,this->boardModel,AdvanceModel::EQUESTRIAN, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::GOVERNMENT,
                              new AdvanceItem(10+168*2,63+25*8,this->boardModel,AdvanceModel::GOVERNMENT, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MEDITATION,
                              new AdvanceItem(10+168*2,63+25*20,this->boardModel,AdvanceModel::MEDITATION, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::ORGANIZED_RELIGION,
                              new AdvanceItem(10+168*2,63+25*23,this->boardModel,AdvanceModel::ORGANIZED_RELIGION, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::SAILS_AND_RIGGINGS,
                              new AdvanceItem(10+168*2,63+25*29+13,this->boardModel,AdvanceModel::SAILS_AND_RIGGINGS, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::SHIPPING,
                              new AdvanceItem(10+168*2,63+25*33,this->boardModel,AdvanceModel::SHIPPING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::ENGINEERING,
                              new AdvanceItem(10+168*2,63+25*39,this->boardModel,AdvanceModel::ENGINEERING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::METAL_WORKING,
                              new AdvanceItem(10+168*2,63+25*42,this->boardModel,AdvanceModel::METAL_WORKING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::SURVEYING,
                              new AdvanceItem(10+168*2,63+25*45,this->boardModel,AdvanceModel::SURVEYING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MEDICINE,
                              new AdvanceItem(10+168*2,63+25*50,this->boardModel,AdvanceModel::MEDICINE, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::WRITTEN_RECORD,
                              new AdvanceItem(10+168*2,63+25*53,this->boardModel,AdvanceModel::WRITTEN_RECORD, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::ARTS,
                              new AdvanceItem(10+168*2,63+25*56,this->boardModel,AdvanceModel::ARTS, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::THEATER,
                              new AdvanceItem(10+168*2,63+25*59,this->boardModel,AdvanceModel::THEATER, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::BANKING,
                              new AdvanceItem(10+168*2,63+25*64,this->boardModel,AdvanceModel::BANKING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::ROADBUILDING,
                              new AdvanceItem(10+168*2,63+25*69,this->boardModel,AdvanceModel::ROADBUILDING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::BLACK_MARKET,
                              new AdvanceItem(10+168*2,63+25*74,this->boardModel,AdvanceModel::BLACK_MARKET, this->advanceItemType));

    this->advanceItemMap.insert(AdvanceModel::CAVALRY,
                              new AdvanceItem(10+168*4,63+25*3,this->boardModel,AdvanceModel::CAVALRY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MILITARY,
                              new AdvanceItem(10+168*4,63+25*8,this->boardModel,AdvanceModel::MILITARY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::DEMOCRACY,
                              new AdvanceItem(10+168*4,63+25*11,this->boardModel,AdvanceModel::DEMOCRACY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::DIPLOMACY,
                              new AdvanceItem(10+168*4,63+25*14,this->boardModel,AdvanceModel::DIPLOMACY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::PHILOSOPHY,
                              new AdvanceItem(10+168*4,63+25*20,this->boardModel,AdvanceModel::PHILOSOPHY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MINISTRY,
                              new AdvanceItem(10+168*4,63+25*23,this->boardModel,AdvanceModel::MINISTRY, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::ARCHITECTURE,
                              new AdvanceItem(10+168*4,63+25*39,this->boardModel,AdvanceModel::ARCHITECTURE, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MACHINING,
                              new AdvanceItem(10+168*4,63+25*42,this->boardModel,AdvanceModel::MACHINING, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::MAGNETICS,
                              new AdvanceItem(10+168*4,63+25*45,this->boardModel,AdvanceModel::MAGNETICS, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::PATRONAGE,
                              new AdvanceItem(10+168*4,63+25*57+13,this->boardModel,AdvanceModel::PATRONAGE, this->advanceItemType));

    this->advanceItemMap.insert(AdvanceModel::CENTRALIZED_GOVERNMENT,
                              new AdvanceItem(10+168*6,63+25*8,this->boardModel,AdvanceModel::CENTRALIZED_GOVERNMENT, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::CIVIL_SERVICE,
                              new AdvanceItem(10+168*6,63+25*11,this->boardModel,AdvanceModel::CIVIL_SERVICE, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::LAW,
                              new AdvanceItem(10+168*6,63+25*17,this->boardModel,AdvanceModel::LAW, this->advanceItemType));
    this->advanceItemMap.insert(AdvanceModel::COMMON_TONGUE,
                              new AdvanceItem(10+168*6,63+25*69,this->boardModel,AdvanceModel::COMMON_TONGUE, this->advanceItemType));

    this->graphicsView->setBackgroundBrush(QBrush(QColor(10,10,10)));
    this->graphicsScene->addItem(this->advanceBackground);
    this->graphicsScene->addItem(this->advanceTitle);
    foreach(AdvanceItem *advanceItem, this->advanceItemMap.values())
    {
        this->graphicsScene->addItem(advanceItem);
    }

    this->graphicsView->move(10,10);

    this->advancesTableDialog = new QDialog(this);
    this->advancesTableDialog->setWindowTitle("Advances Table");
    this->advancesTableLayout = new QGridLayout(this->advancesTableDialog);
    this->advancesTable = new AdvancesTable(this->boardModel, this->advanceItemMap, this);

    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    this->advancesTableDialog->resize(mainScreenSize.width() - 100, mainScreenSize.height() - 60);

    this->resize(mainScreenSize.width() - 50, mainScreenSize.height() - 30);

    this->advancesTableDialog->setLayout(this->advancesTableLayout);
    this->advancesTableLayout->addWidget(this->advancesTable, 0, 0);

    connect(this->tableButton, SIGNAL(clicked()), this, SLOT(showTable()));

    this->advancesTableLayout->addWidget(this->advancesTable);

    this->updateDialog();

    return;
}

AdvanceItem::AdvanceItemType AdvancesDialog::getAdvanceDialogType() const
{
    return this->advanceItemType;
}

void AdvancesDialog::updateDialog()
{
    if(this->advanceItemType == AdvanceItem::SELECTABLE)
    {
        this->setWindowTitle(QString("Select Advances (%1 of %2)")
                                    .arg(this->boardModel->getAdvancesSelected().size())
                                    .arg(this->boardModel->getAdvanceSelectionLimit()));
    }

    foreach(AdvanceItem *item, this->advanceItemMap.values())
    {
        item->updateAdvanceItem();
    }

    return;
}

void AdvancesDialog::selectionDone()
{
    if(this->boardModel->getAdvancesSelected().size() < this->boardModel->getAdvanceSelectionLimit() &&
       this->boardModel->getAdvancesSelected().size() < this->boardModel->getAdvancesAquired().size())
    {
        QMessageBox::StandardButton buttonPressed = QMessageBox::StandardButton(QMessageBox::question(NULL, "Select Advances...", QString("You may select more advances.\nAre you sure you want to gain Glory with only the selected advances?"), QMessageBox::Yes, QMessageBox::No));

        if(buttonPressed != QMessageBox::Yes)
        {
            return;
        }
    }

    this->boardModel->scoreSelectedAdvances();
    this->accept();
    return;
}

void AdvancesDialog::showTable()
{
    this->advancesTable->update();
    this->advancesTableDialog->show();
}

