#include "PocketCivMain.hpp"

#include <QBrush>
#include <QPen>
#include <QScrollBar>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>

#include <iostream>

#include <boost/assert.hpp>

#include "Instruction/ChooseRegionInstruction.hpp"
#include "Instruction/MainPhaseInstruction.hpp"

#include "Instruction/BuildCityInstruction.hpp"
#include "Instruction/BuildFarmInstruction.hpp"
#include "Instruction/ExpeditionInstruction.hpp"

PocketCivMain::PocketCivMain(QWidget *parent) :
    QMainWindow(parent),
    graphicsScene(NULL),
    graphicsView(NULL),
    controlDockWidget("Messages", parent),
    dockWidget(parent),
    dockLayout(NULL),
    boardBackground(NULL),
    boardModel(NULL),
    instruction(NULL)
{
    this->dockLayout = new QGridLayout(&this->dockWidget);

    this->messages = new QPlainTextEdit("To start a new game: File -> New Game",
                                        &this->dockWidget);
    this->messages->setReadOnly(true);

    this->goldCount =       new QLabel("Gold: 0", this);
    this->gloryCount =      new QLabel("Glory: 0", this);
    this->eraCount =        new QLabel("Era: 1", this);
    this->eventCardsLeft =  new QLabel("Event Cards left: 16", this);

    this->buildCity =       new QPushButton("Build City", &this->dockWidget);
    this->buildCity->setEnabled(false);
    connect(this->buildCity, SIGNAL(clicked()), this, SLOT(buildCityTriggered()));
    this->buildFarm =       new QPushButton("Build Farm", &this->dockWidget);
    this->buildFarm->setEnabled(false);
    connect(this->buildFarm, SIGNAL(clicked()), this, SLOT(buildFarmTriggered()));
    this->expedition =      new QPushButton("Expedition", &this->dockWidget);
    this->expedition->setEnabled(false);
    connect(this->expedition, SIGNAL(clicked()), this, SLOT(expeditionTriggered()));
    this->aquireAdvance =   new QPushButton("Aquire Advance", &this->dockWidget);
    this->aquireAdvance->setEnabled(false);
    connect(this->aquireAdvance, SIGNAL(clicked()), this, SLOT(aquireAdvanceTriggered()));
    this->buildWonder =     new QPushButton("Build Wonder", &this->dockWidget);
    this->buildWonder->setEnabled(false);
    connect(this->buildWonder, SIGNAL(clicked()), this, SLOT(buildWonderTriggered()));

    this->collectTaxes =    new QPushButton("Collect Taxes", &this->dockWidget);
    this->collectTaxes->setEnabled(false);
    this->forestation =     new QPushButton("Forestation", &this->dockWidget);
    this->forestation->setEnabled(false);
    this->mining =          new QPushButton("Mining", &this->dockWidget);
    this->mining->setEnabled(false);
    this->overview =        new QPushButton("Overview", &this->dockWidget);
    this->overview->setEnabled(false);
    connect(this->overview, SIGNAL(clicked()), this, SLOT(overviewTriggered()));
    this->done =            new QPushButton("Done", &this->dockWidget);
    this->done->setEnabled(false);
    connect(this->done, SIGNAL(clicked()), this, SLOT(doneTriggered()));

    this->dockLayout->addWidget(this->messages,  0, 0, 1, 5);
    this->dockLayout->addWidget(this->goldCount, 1, 0, 1, 1, Qt::AlignCenter);
    this->dockLayout->addWidget(this->gloryCount, 1, 1, 1, 1, Qt::AlignCenter);
    this->dockLayout->addWidget(this->eraCount, 1, 2, 1, 1, Qt::AlignCenter);
    this->dockLayout->addWidget(this->eventCardsLeft, 1, 3, 1, 2, Qt::AlignCenter);
    this->dockLayout->addWidget(this->buildCity, 2, 0, 1, 1);
    this->dockLayout->addWidget(this->buildFarm, 2, 1, 1, 1);
    this->dockLayout->addWidget(this->expedition, 2, 2, 1, 1);
    this->dockLayout->addWidget(this->aquireAdvance, 2, 3, 1, 1);
    this->dockLayout->addWidget(this->buildWonder, 2, 4, 1, 1);
    this->dockLayout->addWidget(this->collectTaxes, 3, 0, 1, 1);
    this->dockLayout->addWidget(this->forestation, 3, 1, 1, 1);
    this->dockLayout->addWidget(this->mining, 3, 2, 1, 1);
    this->dockLayout->addWidget(this->overview, 3, 3, 1, 1);
    this->dockLayout->addWidget(this->done, 3, 4, 1, 1);

    this->dockWidget.setLayout(this->dockLayout);
    this->controlDockWidget.setFeatures(QDockWidget::DockWidgetFloatable);
    this->controlDockWidget.setAllowedAreas(Qt::RightDockWidgetArea);
    this->controlDockWidget.setWidget(&dockWidget);

    this->addDockWidget(Qt::RightDockWidgetArea, &this->controlDockWidget);

    this->pocketCivMenu = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("File", this);
    this->newGame = new QAction("New Game", fileMenu);
    connect(this->newGame, SIGNAL(triggered()), this, SLOT(newGameTriggered()));
    this->saveGame = new QAction("Save Game", fileMenu);
    connect(this->saveGame, SIGNAL(triggered()), this, SLOT(saveGameTriggered()));
    this->loadGame = new QAction("Load Game", fileMenu);
    connect(this->loadGame, SIGNAL(triggered()), this, SLOT(loadGameTriggered()));

    fileMenu->addAction(this->newGame);
    fileMenu->addSeparator();
    fileMenu->addAction(this->saveGame);
    fileMenu->addAction(this->loadGame);
    this->pocketCivMenu->addMenu(fileMenu);
    QMenu *optionsMenu = new QMenu("Options", this);
    this->pocketCivMenu->addMenu(optionsMenu);
    this->setMenuBar(this->pocketCivMenu);

    this->generateNewBoard(new BoardModel(20,10, this));
}

PocketCivMain::~PocketCivMain()
{
    this->clearBoard();
}

void PocketCivMain::generateNewBoard(BoardModel *boardModel)
{
    assert(boardModel != NULL);

    if(this->boardModel != NULL)
    {
        delete this->boardModel;
    }

    this->boardModel = boardModel;
    int width = this->boardModel->getWidth();
    int height = this->boardModel->getHeight();
    double sceneWidth = 37.5*double(width);
    double sceneHeight = 50.0*double(height);

    if(this->graphicsScene != NULL)
    {
        this->graphicsScene->deleteLater();
    }

    this->graphicsScene = new QGraphicsScene(0,0,sceneWidth,sceneHeight, this);

    if(this->graphicsView != NULL)
    {
        this->graphicsView->deleteLater();
    }
    this->graphicsView = new QGraphicsView(this->graphicsScene, this);
    this->setCentralWidget(this->graphicsView);

    this->clearBoard();

    for(int i = 0; i < width; ++i)
    {
        this->hexItems.append(QList<HexItem *>());
        for(int j = 0; j < height; ++j)
        {
            this->hexItems[i].append(NULL);
        }
    }

    if(this->boardBackground != NULL)
    {
        delete this->boardBackground;
    }

    this->boardBackground = new QGraphicsRectItem(HexItem::BOARD_BORDER-1, HexItem::BOARD_BORDER-1, HexItem::BOARD_BORDER+width*34-14, HexItem::BOARD_BORDER+height*40-7);
    this->boardBackground->setPen(QPen(QColor(0xb1,0xb1,0xb1)));
    this->boardBackground->setBrush(QBrush(QColor(0xb1,0xb1,0xb1)));
    this->graphicsScene->addItem(this->boardBackground);

    int x = 0;
    int y = 0;
    for(int i = 1; i <= width; i=i+2)
    {
        y = 0;
        for(int j = 1; j <= height; ++j)
        {
            HexModel *hexModel = this->boardModel->refHexModel(x,y);
            connect(hexModel, SIGNAL(hexModelUpdated(int, int)), this, SLOT(updateHex(int,int)));
            HexItem *hexItem = new HexItem(i*34-34,j*40-20, hexModel);
            this->graphicsScene->addItem(hexItem);
            this->hexItems[x][y] = hexItem;
            connect(hexModel, SIGNAL(hexTriggered(Qt::MouseButton,int,int)), this, SLOT(hexTriggerAction(Qt::MouseButton,int,int)));
            y++;
        }
        x = x + 2;
    }

    x = 1;
    for(int i = 1; i <= width; i=i+2)
    {
        y = 0;
        for(int j = 1; j <= height; ++j)
        {
            HexModel *hexModel = this->boardModel->refHexModel(x,y);
            connect(hexModel, SIGNAL(hexModelUpdated(int, int)), this, SLOT(updateHex(int,int)));
            HexItem *hexItem = new HexItem(i*34,j*40-40, hexModel);
            this->graphicsScene->addItem(hexItem);
            this->hexItems[x][y] = hexItem;
            connect(hexModel, SIGNAL(hexTriggered(Qt::MouseButton,int,int)), this, SLOT(hexTriggerAction(Qt::MouseButton,int,int)));
            y++;
        }
        x = x + 2;
    }

    connect(this->boardModel, SIGNAL(boardUpdated()), this, SLOT(updateBoard()));
    connect(this->boardModel, SIGNAL(boardCleared()), this, SLOT(clearBoard()));
    connect(this->boardModel, SIGNAL(sendMessage(const QString &)), this, SLOT(addMessage(const QString &)));
    connect(this->boardModel, SIGNAL(clearMessages()), this, SLOT(clearMessages()));
    connect(this->boardModel, SIGNAL(sendCardsLeftCount(int)), this, SLOT(setEventCardsLeft(int)));
    connect(this->boardModel, SIGNAL(sendDoneText(const QString &)), this, SLOT(setDoneText(const QString &)));
    connect(this->boardModel, SIGNAL(sendDialogClosed()), this, SLOT(continueWithPreviousInstruction()));
    connect(this->boardModel, SIGNAL(eraChanged(int)), this, SLOT(setEra(int)));
    connect(this->boardModel, SIGNAL(goldChanged(int)), this, SLOT(setGoldCount(int)));
    connect(this->boardModel, SIGNAL(gloryScoreChanged(int)), this, SLOT(setGloryCount(int)));

    if(this->instruction != NULL)
    {
        this->instruction->deleteLater();
    }

    this->instruction = new NoInstruction();
    this->updateBoard();
}

void PocketCivMain::processInstruction(Instruction *nextInstruction)
{
    if(nextInstruction == NULL)
    {
        std::cout << "Instruction failed." << std::endl;
        this->instruction = new NoInstruction();
        return;
    }

    if(this->instruction != nextInstruction && !this->instruction->keepInstruction())
    {
        this->instruction->deleteLater();
    }

    this->instruction = nextInstruction;

    this->updateBoard();
    return;
}

void PocketCivMain::clearBoard()
{
    foreach(QList<HexItem *> list, this->hexItems)
    {
        foreach(HexItem *item, list)
        {
            delete item;
        }
    }

    this->hexItems.clear();
    return;
}

void PocketCivMain::updateHex(HexItem *hexItem)
{
    assert(hexItem != NULL);

    hexItem->updateHexItem();

    return;
}

void PocketCivMain::updateHex(int x, int y)
{
    assert(x >= 0 && x < this->hexItems.size());
    assert(y >= 0 && y < this->hexItems[x].size());

    this->updateHex(this->hexItems[x][y]);
    return;
}



void PocketCivMain::hexTriggerAction(Qt::MouseButton button, int x, int y)
{
    this->processInstruction(this->instruction->triggerHex(button, x, y));
    return;
}

void PocketCivMain::continueWithPreviousInstruction()
{
    this->processInstruction(this->instruction->getFollowingInstruction());
    return;
}

void PocketCivMain::addMessage(const QString &message)
{
    this->messages->appendPlainText(message);
    this->messages->verticalScrollBar()->setSliderPosition(this->messages->verticalScrollBar()->maximum());
    return;
}

void PocketCivMain::clearMessages()
{
    this->messages->clear();
    return;
}

void PocketCivMain::setDoneText(const QString &text)
{
    this->done->setText(text);
    return;
}

void PocketCivMain::setGoldCount(int goldCount)
{
    this->goldCount->setText(QString("Gold: %1").arg(QString::number(goldCount)));
    return;
}

void PocketCivMain::setGloryCount(int gloryCount)
{
    this->gloryCount->setText(QString("Glory: %1").arg(QString::number(gloryCount)));
    return;
}

void PocketCivMain::setEra(int era)
{
    this->eraCount->setText(QString("Era: %1").arg(QString::number(era)));
    return;
}

void PocketCivMain::setEventCardsLeft(int eventCardsLeft)
{
    this->eventCardsLeft->setText(QString("Event Cards left: %1").arg(QString::number(eventCardsLeft)));
    return;
}

void PocketCivMain::newGameTriggered()
{
    this->generateNewBoard(new BoardModel(20,10,this));

    if(this->instruction != NULL)
    {
        this->instruction->deleteLater();
    }

    this->boardModel->setGold(0);
    this->boardModel->setGloryScore(0);
    this->boardModel->setEra(1);
    this->boardModel->reshuffleCards();

    this->overview->setEnabled(true);
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();
    this->done->setText("Done");

    this->messages->clear();
    this->addMessage("Started a new game!!");
    this->addMessage(" ");
    this->addMessage("WORLD GENERATION:");
    this->addMessage(QString("Select two or more connected hexes on the board to form Region %1 out of %2.").
                                 arg(1).arg(8));
    this->addMessage("When you are done, press Done...");

    this->instruction = new ChooseRegionInstruction(this->boardModel, 1, 8);
    this->instruction->initInstruction();
    this->updateBoard();
    return;
}

void PocketCivMain::saveGameTriggered()
{
    this->processInstruction(this->instruction->triggerSaveGame());
    return;
}

void PocketCivMain::loadGameTriggered()
{
    QString loadFile =
    QFileDialog::getOpenFileName(NULL,
                                 "Open Game...",
                                 QDir::current().path(),
                                 "PocketCiv Saves (*.pcsave)");

    QFile file(loadFile);
    file.open(QFile::ReadOnly);
    QDataStream reader(&file);

    BoardModel *boardModel = new BoardModel(20,10,this);
    boardModel->deserialize(reader);

    this->generateNewBoard(boardModel);

    this->overview->setEnabled(true);
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();
    this->done->setText("Done");

    this->messages->clear();
    this->addMessage("Loaded a game!!");
    this->addMessage(" ");

    if(this->instruction != NULL)
    {
        this->instruction->deleteLater();
    }

    this->instruction = new MainPhaseInstruction(this->boardModel);
    this->instruction->initInstruction();
    this->updateBoard();

    return;
}

void PocketCivMain::buildCityTriggered()
{
    Instruction *buildCityInstruction = new BuildCityInstruction(this->boardModel, this->instruction);
    this->processInstruction(buildCityInstruction);
    return;
}

void PocketCivMain::buildFarmTriggered()
{
    Instruction *buildFarmInstruction = new BuildFarmInstruction(this->boardModel, this->instruction);
    this->processInstruction(buildFarmInstruction);
    return;
}

void PocketCivMain::expeditionTriggered()
{
    Instruction *expeditionInstruction = new ExpeditionInstruction(this->boardModel, this->instruction);
    this->processInstruction(expeditionInstruction);
    return;
}

void PocketCivMain::aquireAdvanceTriggered()
{
    // TODO: Do it.
    //this->processInstruction(...);
    return;
}

void PocketCivMain::buildWonderTriggered()
{
    // TODO: Do it.
    //this->processInstruction(...);
    return;
}

void PocketCivMain::overviewTriggered()
{
    // TODO: Do it.
    //this->processInstruction(...);
    return;
}

void PocketCivMain::doneTriggered()
{
    this->processInstruction(this->instruction->triggerDone());
    return;
}

void PocketCivMain::updateBoard()
{
    foreach(QList<HexItem *> list, this->hexItems)
    {
        foreach(HexItem *item, list)
        {
            this->updateHex(item);
        }
    }

    this->buildCity->setEnabled(this->boardModel->canBuildCity());
    this->buildFarm->setEnabled(this->boardModel->canBuildFarm());
    this->expedition->setEnabled(this->boardModel->canDoExpedition());
    this->aquireAdvance->setEnabled(this->boardModel->canAquireAdvance());
    this->buildWonder->setEnabled(this->boardModel->canBuildWonder());
    this->collectTaxes->setEnabled(this->boardModel->canCollectTaxes());
    this->forestation->setEnabled(this->boardModel->canDoForestation());
    this->mining->setEnabled(this->boardModel->canDoMining());
    this->done->setEnabled(this->boardModel->isDoneEnabled());

    return;
}

void PocketCivMain::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        this->doneTriggered();
    }
}
