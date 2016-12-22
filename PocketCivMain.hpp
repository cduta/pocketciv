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
#include "OverviewDialog.hpp"
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
    QPushButton *overview;
    QPushButton *done;       // May also show other things like continue, ...

    QMenuBar *pocketCivMenu;
    QAction *newGame;
    QAction *saveGame;
    QAction *loadGame;

    QList<QList<HexItem *> > hexItems;
    QGraphicsRectItem *boardBackground;

    OverviewDialog *overviewDialog;

    BoardModel *boardModel;
    Instruction *instruction;

public:
    PocketCivMain(QWidget *parent = 0);
    ~PocketCivMain();

private:
    void generateNewBoard(BoardModel *boardModel);
    void processInstruction(Instruction *nextInstruction);

private slots:
    void clearBoard();
    void updateHex(HexItem *hexItem);
    void updateHex(int x, int y);
    void hexTriggerAction(Qt::MouseButton button, int x, int y);
    void continueWithPreviousInstruction();
    void addMessage(const QString &message);
    void clearMessages();
    void setDoneText(const QString &text);

    void setGoldCount(int goldCount);
    void setGloryCount(int gloryCount);
    void setEra(int era);
    void setEventCardsLeft(int eventCardsLeft);

    void newGameTriggered();
    void saveGameTriggered();
    void loadGameTriggered();

    void buildCityTriggered();
    void buildFarmTriggered();
    void expeditionTriggered();
    void forestationTriggered();
    void miningTriggered();
    void aquireAdvanceTriggered();
    void buildWonderTriggered();
    void collectTaxesTriggered();

    void overviewTriggered();
    void doneTriggered();
    void updateBoard();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // _POCKET_CIV_MAIN_HPP_
