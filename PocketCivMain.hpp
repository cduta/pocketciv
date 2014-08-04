#ifndef _POCKET_CIV_MAIN_HPP_
#define _POCKET_CIV_MAIN_HPP_

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QDockWidget>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QAction>
#include <QLabel>

#include <iostream>

#include "BoardModel.hpp"
#include "HexItem.hpp"
#include "HexModel.hpp"
#include "Instruction/Instruction.hpp"

#include <boost/assert.hpp>

#include <QDebug>

class PocketCivMain : public QMainWindow
{
    Q_OBJECT

    QGraphicsScene *graphicsScene;
    QGraphicsView *graphicsView;
    QDockWidget controlDockWidget;

    QWidget dockWidget;
    QGridLayout *dockLayout;
    QPlainTextEdit *messages;
    QLabel *goldCount;
    QLabel *gloryCount;
    QLabel *eraCount;
    QLabel *eventCardsLeft;
    QPushButton *buildCity;
    QPushButton *buildFarm;
    QPushButton *expedition;
    QPushButton *aquireAdvance; // May also show advances.
    QPushButton *buildWonder;   // May also show wonders.

    QPushButton *collectTaxes;
    QPushButton *forestation;
    QPushButton *mining;
//    QPushButton *undo;
    QPushButton *done;       // May also show other things like continue, ...

    QMenuBar *pocketCivMenu;
    QAction *newGame;

    QList<QList<HexItem *> > hexItems;
    QGraphicsRectItem *boardBackground;

    BoardModel *boardModel;
    Instruction *instruction;

public:
    PocketCivMain(QWidget *parent = 0);
    ~PocketCivMain();

private:
    void generateNewBoard(BoardModel *boardModel);

private slots:
    void clearBoard();
    void updateHex(HexItem *hexItem);
    void updateHex(int x, int y);
    void hexTriggerAction(Qt::MouseButton button, int x, int y);
    void addMessage(const QString &message);

    void newGameTriggered();

    void buildCityTriggered();
    void doneTriggered();
    void updateBoard();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // _POCKET_CIV_MAIN_HPP_
