#ifndef _POCKET_CIV_MAIN_HPP_
#define _POCKET_CIV_MAIN_HPP_

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

#include <iostream>

#include "BoardModel.hpp"
#include "HexItem.hpp"
#include "HexModel.hpp"

#include <boost/assert.hpp>

#include <QDebug>

class PocketCivMain : public QMainWindow
{
    Q_OBJECT

    QGraphicsScene graphicsScene;
    QGraphicsView graphicsView;
    QList<QList<HexItem *> > hexItems;
    QGraphicsRectItem *boardBackground;

    BoardModel *boardModel;

public:
    PocketCivMain(QWidget *parent = 0);
    ~PocketCivMain();

private:
    void generateNewBoard(BoardModel *boardModel);

private slots:
    void clearBoard();
    void updateHex(HexItem *hexItem);
    void updateHex(int x, int y);
    void updateBoard();
};

#endif // _POCKET_CIV_MAIN_HPP_
