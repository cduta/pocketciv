#ifndef CARDDIALOG_HPP
#define CARDDIALOG_HPP

#include <QDialog>
#include <QGridLayout>
#include <QListWidget>
#include <QWidget>

#include "BoardModel.hpp"

class CardDialog : public QDialog
{
    Q_OBJECT

    BoardModel *boardModel;

    QGridLayout *cardLayout;
    QListWidget *cardsLeft;
    QListWidget *cardsDiscarded;

public:
    CardDialog(BoardModel *boardModel, QWidget *parent = 0);

    virtual void show();
};

#endif // CARDDIALOG_HPP
