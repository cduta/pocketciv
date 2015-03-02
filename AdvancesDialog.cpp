#include "AdvancesDialog.hpp"

AdvancesDialog::AdvancesDialog(BoardModel *boardModel, AdvanceDialogType dialogType, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      activeRegion(-1),
      dialogType(dialogType),
      selectionLimit(1),
      layout(new QGridLayout(this))
{
    assert(dialogType != AQUIRE_ADVANCE);
    this->init();
}

AdvancesDialog::AdvancesDialog(BoardModel *boardModel, int activeRegion, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      activeRegion(activeRegion),
      dialogType(AQUIRE_ADVANCE),
      selectionLimit(1)
{
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

void AdvancesDialog::init()
{
    connect(this->boardModel, SIGNAL(boardUpdated()), this, SLOT(updateDialog()));

    this->graphicsScene = new QGraphicsScene(this);
    this->graphicsView = new QGraphicsView(this->graphicsScene, this);
    this->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->layout->addWidget(this->graphicsView, 0,0);
    this->setLayout(this->layout);
    this->resize(650,450);

    this->advanceBackground = new QGraphicsPixmapItem(QPixmap(":/advance_background"));
    this->advanceBackground->setPos(0,0);

    this->advanceTitle = new QGraphicsPixmapItem(QPixmap(":/advance_title"));
    this->advanceTitle->setPos(0,0);

    this->advanceItemMap.insert(AdvanceModel::AGRICULTURE,
                              new AdvanceItem(10+0,63+0,this->boardModel,AdvanceModel::AGRICULTURE));
    this->advanceItemMap.insert(AdvanceModel::HORTICULTURE,
                              new AdvanceItem(10+0,63+25*3,this->boardModel,AdvanceModel::HORTICULTURE));
    this->advanceItemMap.insert(AdvanceModel::SENSE_OF_COMMUNITY,
                              new AdvanceItem(10+0,63+25*8,this->boardModel,AdvanceModel::SENSE_OF_COMMUNITY));
    this->advanceItemMap.insert(AdvanceModel::MYTHOLOGY,
                              new AdvanceItem(10+0,63+(25*20),this->boardModel,AdvanceModel::MYTHOLOGY));
    this->advanceItemMap.insert(AdvanceModel::ASTRONOMY,
                              new AdvanceItem(10+0,63+(25*28),this->boardModel,AdvanceModel::ASTRONOMY));
    this->advanceItemMap.insert(AdvanceModel::NAVIGATION,
                              new AdvanceItem(10+0,63+(25*31),this->boardModel,AdvanceModel::NAVIGATION));
    this->advanceItemMap.insert(AdvanceModel::FISHING,
                              new AdvanceItem(10+0,63+(25*34),this->boardModel,AdvanceModel::FISHING));
    this->advanceItemMap.insert(AdvanceModel::MASONRY,
                              new AdvanceItem(10+0,63+(25*39),this->boardModel,AdvanceModel::MASONRY));
    this->advanceItemMap.insert(AdvanceModel::MINING,
                              new AdvanceItem(10+0,63+(25*42),this->boardModel,AdvanceModel::MINING));
    this->advanceItemMap.insert(AdvanceModel::LITERACY,
                              new AdvanceItem(10+0,63+(25*56),this->boardModel,AdvanceModel::LITERACY));
    this->advanceItemMap.insert(AdvanceModel::MUSIC,
                              new AdvanceItem(10+0,63+(25*59),this->boardModel,AdvanceModel::MUSIC));
    this->advanceItemMap.insert(AdvanceModel::COINAGE,
                              new AdvanceItem(10+0,63+(25*64),this->boardModel,AdvanceModel::COINAGE));
    this->advanceItemMap.insert(AdvanceModel::CARTAGE,
                              new AdvanceItem(10+0,63+(25*69),this->boardModel,AdvanceModel::CARTAGE));
    this->advanceItemMap.insert(AdvanceModel::CULTURE_OF_THIEVES,
                              new AdvanceItem(10+0,63+(25*74),this->boardModel,AdvanceModel::CULTURE_OF_THIEVES));
    this->advanceItemMap.insert(AdvanceModel::SIMPLE_TOOLS,
                              new AdvanceItem(10+0,63+(25*79),this->boardModel,AdvanceModel::SIMPLE_TOOLS));
    this->advanceItemMap.insert(AdvanceModel::BASIC_TOOLS,
                              new AdvanceItem(10+0,63+(25*84),this->boardModel,AdvanceModel::BASIC_TOOLS));
    this->advanceItemMap.insert(AdvanceModel::SLAVE_LABOR,
                              new AdvanceItem(10+0,63+(25*89),this->boardModel,AdvanceModel::SLAVE_LABOR));
    this->advanceItemMap.insert(AdvanceModel::STORY_TELLING,
                              new AdvanceItem(10+0,63+(25*94),this->boardModel,AdvanceModel::STORY_TELLING));

    this->advanceItemMap.insert(AdvanceModel::IRRIGATION,
                              new AdvanceItem(10+168*2,63+25*0,this->boardModel,AdvanceModel::IRRIGATION));
    this->advanceItemMap.insert(AdvanceModel::EQUESTRIAN,
                              new AdvanceItem(10+168*2,63+25*3,this->boardModel,AdvanceModel::EQUESTRIAN));
    this->advanceItemMap.insert(AdvanceModel::GOVERNMENT,
                              new AdvanceItem(10+168*2,63+25*8,this->boardModel,AdvanceModel::GOVERNMENT));
    this->advanceItemMap.insert(AdvanceModel::MEDITATION,
                              new AdvanceItem(10+168*2,63+25*20,this->boardModel,AdvanceModel::MEDITATION));
    this->advanceItemMap.insert(AdvanceModel::ORGANIZED_RELIGION,
                              new AdvanceItem(10+168*2,63+25*23,this->boardModel,AdvanceModel::ORGANIZED_RELIGION));
    this->advanceItemMap.insert(AdvanceModel::SAILS_AND_RIGGINGS,
                              new AdvanceItem(10+168*2,63+25*29+13,this->boardModel,AdvanceModel::SAILS_AND_RIGGINGS));
    this->advanceItemMap.insert(AdvanceModel::SHIPPING,
                              new AdvanceItem(10+168*2,63+25*33,this->boardModel,AdvanceModel::SHIPPING));
    this->advanceItemMap.insert(AdvanceModel::ENGINEERING,
                              new AdvanceItem(10+168*2,63+25*39,this->boardModel,AdvanceModel::ENGINEERING));
    this->advanceItemMap.insert(AdvanceModel::METAL_WORKING,
                              new AdvanceItem(10+168*2,63+25*42,this->boardModel,AdvanceModel::METAL_WORKING));
    this->advanceItemMap.insert(AdvanceModel::SURVEYING,
                              new AdvanceItem(10+168*2,63+25*45,this->boardModel,AdvanceModel::SURVEYING));
    this->advanceItemMap.insert(AdvanceModel::MEDICINE,
                              new AdvanceItem(10+168*2,63+25*50,this->boardModel,AdvanceModel::MEDICINE));
    this->advanceItemMap.insert(AdvanceModel::WRITTEN_RECORD,
                              new AdvanceItem(10+168*2,63+25*53,this->boardModel,AdvanceModel::WRITTEN_RECORD));
    this->advanceItemMap.insert(AdvanceModel::ARTS,
                              new AdvanceItem(10+168*2,63+25*56,this->boardModel,AdvanceModel::ARTS));
    this->advanceItemMap.insert(AdvanceModel::THEATER,
                              new AdvanceItem(10+168*2,63+25*59,this->boardModel,AdvanceModel::THEATER));
    this->advanceItemMap.insert(AdvanceModel::BANKING,
                              new AdvanceItem(10+168*2,63+25*64,this->boardModel,AdvanceModel::BANKING));
    this->advanceItemMap.insert(AdvanceModel::ROADBUILDING,
                              new AdvanceItem(10+168*2,63+25*69,this->boardModel,AdvanceModel::ROADBUILDING));
    this->advanceItemMap.insert(AdvanceModel::BLACK_MARKET,
                              new AdvanceItem(10+168*2,63+25*74,this->boardModel,AdvanceModel::BLACK_MARKET));

    this->advanceItemMap.insert(AdvanceModel::CAVALRY,
                              new AdvanceItem(10+168*4,63+25*3,this->boardModel,AdvanceModel::CAVALRY));
    this->advanceItemMap.insert(AdvanceModel::MILITARY,
                              new AdvanceItem(10+168*4,63+25*8,this->boardModel,AdvanceModel::MILITARY));
    this->advanceItemMap.insert(AdvanceModel::DEMOCRACY,
                              new AdvanceItem(10+168*4,63+25*11,this->boardModel,AdvanceModel::DEMOCRACY));
    this->advanceItemMap.insert(AdvanceModel::DIPLOMACY,
                              new AdvanceItem(10+168*4,63+25*14,this->boardModel,AdvanceModel::DIPLOMACY));
    this->advanceItemMap.insert(AdvanceModel::PHILOSOPHY,
                              new AdvanceItem(10+168*4,63+25*20,this->boardModel,AdvanceModel::PHILOSOPHY));
    this->advanceItemMap.insert(AdvanceModel::MINISTRY,
                              new AdvanceItem(10+168*4,63+25*23,this->boardModel,AdvanceModel::MINISTRY));
    this->advanceItemMap.insert(AdvanceModel::ARCHITECTURE,
                              new AdvanceItem(10+168*4,63+25*39,this->boardModel,AdvanceModel::ARCHITECTURE));
    this->advanceItemMap.insert(AdvanceModel::MACHINING,
                              new AdvanceItem(10+168*4,63+25*42,this->boardModel,AdvanceModel::MACHINING));
    this->advanceItemMap.insert(AdvanceModel::MAGNETICS,
                              new AdvanceItem(10+168*4,63+25*45,this->boardModel,AdvanceModel::MAGNETICS));
    this->advanceItemMap.insert(AdvanceModel::PATRONAGE,
                              new AdvanceItem(10+168*4,63+25*57+13,this->boardModel,AdvanceModel::PATRONAGE));

    this->advanceItemMap.insert(AdvanceModel::CENTRALIZED_GOVERNMENT,
                              new AdvanceItem(10+168*6,63+25*8,this->boardModel,AdvanceModel::CENTRALIZED_GOVERNMENT));
    this->advanceItemMap.insert(AdvanceModel::CIVIL_SERVICE,
                              new AdvanceItem(10+168*6,63+25*11,this->boardModel,AdvanceModel::CIVIL_SERVICE));
    this->advanceItemMap.insert(AdvanceModel::LAW,
                              new AdvanceItem(10+168*6,63+25*17,this->boardModel,AdvanceModel::LAW));
    this->advanceItemMap.insert(AdvanceModel::COMMON_TONGUE,
                              new AdvanceItem(10+168*6,63+25*69,this->boardModel,AdvanceModel::COMMON_TONGUE));

    //this->graphicsView->setBackgroundBrush(QBrush(QColor(0,0,0)));
    this->graphicsScene->addItem(this->advanceBackground);
    this->graphicsScene->addItem(this->advanceTitle);
    foreach(AdvanceItem *advanceItem, this->advanceItemMap.values())
    {
        this->graphicsScene->addItem(advanceItem);
    }

    this->graphicsView->move(10,10);

    this->updateDialog();

    connect(this->boardModel, SIGNAL(advanceAquired(AdvanceModel::Advance)), this, SLOT(updateDialog()));
    connect(this->boardModel, SIGNAL(goldChanged(int)), this, SLOT(updateDialog()));

    return;
}

void AdvancesDialog::setSelectionLimit(int selectionLimit)
{
    this->selectionLimit = selectionLimit;
    return;
}

AdvancesDialog::AdvanceDialogType AdvancesDialog::getAdvanceDialogType() const
{
    return this->dialogType;
}

void AdvancesDialog::updateDialog()
{
    foreach(AdvanceItem *item, this->advanceItemMap.values())
    {
        item->updateAdvanceItem();
    }

    return;
}

