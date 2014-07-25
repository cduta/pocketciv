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

PocketCivMain::PocketCivMain(QWidget *parent) :
    QMainWindow(parent), graphicsScene(0,0,750,500, parent), graphicsView(&this->graphicsScene, parent)
{
    this->setCentralWidget(&this->graphicsView);
    this->resize(800, 550);
    this->generateNewBoard(new BoardModel(20,10, this));
}

PocketCivMain::~PocketCivMain()
{
    this->clearBoard();
}

void PocketCivMain::generateNewBoard(BoardModel *boardModel)
{
    assert(boardModel != NULL);

    this->boardModel = boardModel;
    this->boardModel->initializeBoard();
    int width = this->boardModel->getWidth();
    int height = this->boardModel->getHeight();

    this->clearBoard();

    for(int i = 0; i < width; ++i)
    {
        this->hexItems.append(QList<HexItem *>());
        for(int j = 0; j < height; ++j)
        {
            this->hexItems[i].append(NULL);
        }
    }

    this->boardBackground = new QGraphicsRectItem(HexItem::BOARD_BORDER-1, HexItem::BOARD_BORDER-1, HexItem::BOARD_BORDER+width*34-14, HexItem::BOARD_BORDER+height*40-7);
    this->boardBackground->setPen(QPen(QColor(0xb1,0xb1,0xb1)));
    this->boardBackground->setBrush(QBrush(QColor(0xb1,0xb1,0xb1)));
    this->graphicsScene.addItem(this->boardBackground);

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
            this->graphicsScene.addItem(hexItem);
            this->hexItems[x][y] = hexItem;
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
            this->graphicsScene.addItem(hexItem);
            this->hexItems[x][y] = hexItem;
            y++;
        }
        x = x + 2;
    }

//    x = 0;
//    y = 0;
//    for(int i = 1; i <= width; i=i+2)
//    {
//        y = 0;
//        for(int j = 1; j <= height; ++j)
//        {
//            HexModel *hexModel = this->boardModel->refHexModel(x,y);
//            connect(hexModel, SIGNAL(hexModelUpdated(int, int)), this, SLOT(updateHex(int,int)));
//            HexItem *hexItem = new HexItem(i*34-34,j*40-20, hexModel);
//            this->graphicsScene.addItem(hexItem);
//            this->hexItems[x][y] = hexItem;
//            y++;
//        }
//        x = x + 2;
//    }

//    x = 1;
//    for(int i = 1; i <= width; i=i+2)
//    {
//        y = 0;
//        for(int j = 1; j <= height; ++j)
//        {
//            HexModel *hexModel = this->boardModel->refHexModel(x,y);
//            connect(hexModel, SIGNAL(hexModelUpdated(int, int)), this, SLOT(updateHex(int,int)));
//            HexItem *hexItem = new HexItem(i*34,j*40-40, hexModel);
//            this->graphicsScene.addItem(hexItem);
//            this->hexItems[x][y] = hexItem;
//            y++;
//        }
//        x = x + 2;
//    }

    connect(this->boardModel, SIGNAL(boardUpdated()), this, SLOT(updateBoard()));
    connect(this->boardModel, SIGNAL(boardCleared()), this, SLOT(clearBoard()));
}

void PocketCivMain::clearBoard()
{
    foreach(QList<HexItem *> list, this->hexItems)
    {
        foreach(HexItem *item, list)
        {
            this->graphicsScene.removeItem(item);
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
