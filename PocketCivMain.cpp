#include "PocketCivMain.hpp"

#include <QBrush>
#include <QPen>
#include <QScrollBar>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <iostream>

#include <boost/assert.hpp>

#include "Instruction/ChooseRegionInstruction.hpp"

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
    this->generateNewBoard(new BoardModel(20,10, this));

    this->dockLayout = new QGridLayout(&this->dockWidget);

    this->messages = new QPlainTextEdit("To start a new game: File -> New Game",
                                        &this->dockWidget);
    this->messages->setReadOnly(true);

    this->buildCity =       new QPushButton("Build City", &this->dockWidget);
    this->buildCity->setEnabled(false);
    connect(this->buildCity, SIGNAL(clicked()), this, SLOT(buildCityTriggered()));
    this->buildFarm =       new QPushButton("Build Farm", &this->dockWidget);
    this->buildFarm->setEnabled(false);
    this->expedition =      new QPushButton("Expedition", &this->dockWidget);
    this->expedition->setEnabled(false);
    this->aquireAdvance =   new QPushButton("Aquire Advance", &this->dockWidget);
    this->aquireAdvance->setEnabled(false);
    this->buildWonder =     new QPushButton("Build Wonder", &this->dockWidget);
    this->buildWonder->setEnabled(false);

    this->collectTaxes =    new QPushButton("Collect Taxes", &this->dockWidget);
    this->collectTaxes->setEnabled(false);
    this->forestation =     new QPushButton("Forestation", &this->dockWidget);
    this->forestation->setEnabled(false);
    this->mining =          new QPushButton("Mining", &this->dockWidget);
    this->mining->setEnabled(false);
//    this->undo =            new QPushButton("Undo", &this->dockWidget);
//    this->undo->setEnabled(false);
    this->done =            new QPushButton("Done", &this->dockWidget);
    this->done->setEnabled(false);
    connect(this->done, SIGNAL(clicked()), this, SLOT(doneTriggered()));

    this->dockLayout->addWidget(this->messages,  0, 0, 1, 5);
    this->dockLayout->addWidget(this->buildCity, 1, 0, 1, 1);
    this->dockLayout->addWidget(this->buildFarm, 1, 1, 1, 1);
    this->dockLayout->addWidget(this->expedition, 1, 2, 1, 1);
    this->dockLayout->addWidget(this->aquireAdvance, 1, 3, 1, 1);
    this->dockLayout->addWidget(this->buildWonder, 1, 4, 1, 1);
    this->dockLayout->addWidget(this->collectTaxes, 2, 0, 1, 1);
    this->dockLayout->addWidget(this->forestation, 2, 1, 1, 1);
    this->dockLayout->addWidget(this->mining, 2, 2, 1, 1);
//    this->dockLayout->addWidget(this->undo, 2, 3, 1, 1);
    this->dockLayout->addWidget(this->done, 2, 4, 1, 1);

    this->dockWidget.setLayout(this->dockLayout);
    this->controlDockWidget.setFeatures(QDockWidget::DockWidgetFloatable);
    this->controlDockWidget.setAllowedAreas(Qt::RightDockWidgetArea);
    this->controlDockWidget.setWidget(&dockWidget);

    this->addDockWidget(Qt::RightDockWidgetArea, &this->controlDockWidget);

    this->pocketCivMenu = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("File", this);
    this->newGame = new QAction("New Game", fileMenu);
    connect(this->newGame, SIGNAL(triggered()), this, SLOT(newGameTriggered()));
    fileMenu->addAction(this->newGame);
    this->pocketCivMenu->addMenu(fileMenu);
    QMenu *optionsMenu = new QMenu("Options", this);
    this->pocketCivMenu->addMenu(optionsMenu);
    this->setMenuBar(this->pocketCivMenu);
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

    if(this->instruction != NULL)
    {
        this->instruction->deleteLater();
    }

    this->instruction = new NoInstruction(this->boardModel, this);
    this->updateBoard();
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
    Instruction *nextInstruction = this->instruction->triggerHex(button, x, y);

    if(nextInstruction == NULL)
    {
        std::cout << "Instruction failed." << std::endl;
        return;
    }

    if(this->instruction != nextInstruction)
    {
        this->instruction->deleteLater();
        this->instruction = nextInstruction;
    }

    this->updateBoard();
    return;
}

void PocketCivMain::addMessage(const QString &message)
{
    this->messages->appendPlainText(message);
    return;
}

void PocketCivMain::newGameTriggered()
{
    this->generateNewBoard(new BoardModel(20,10,this));

    if(this->instruction != NULL)
    {
        this->instruction->deleteLater();
    }

    this->buildCity->setEnabled(false);
    this->buildFarm->setEnabled(false);
    this->expedition->setEnabled(false);
    this->aquireAdvance->setEnabled(false);
    this->buildWonder->setEnabled(false);
    this->collectTaxes->setEnabled(false);
    this->forestation->setEnabled(false);
    this->mining->setEnabled(false);
    this->done->setEnabled(true);
    this->done->setText("Done");

    this->messages->clear();
    this->addMessage("Started a new game!!");
    this->addMessage("World Generation:");
    this->addMessage(QString("Select two or more connected hexes on the board to form Region %1 out of %2.").
                                 arg(1).arg(8));
    this->addMessage("When you are done, press Next...");
    this->addMessage("Region 1 out of 8.");

    this->instruction = new ChooseRegionInstruction(this->boardModel, 1, 8, this);
    this->updateBoard();
    return;
}

void PocketCivMain::buildCityTriggered()
{
    // TODO: Trigger to process build city instruction to build city.
    return;
}

void PocketCivMain::doneTriggered()
{
    this->instruction = this->instruction->triggerDone();
    this->updateBoard();
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
    return;
}
