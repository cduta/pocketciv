#ifndef GLORYDIALOG_HPP
#define GLORYDIALOG_HPP

#include <QDialog>
#include <QGridLayout>
#include <QPlainTextEdit>

#include "BoardModel.hpp"

class GloryDialog : public QDialog
{
    Q_OBJECT

    BoardModel *boardModel;

    QGridLayout *gridLayout;
    QPlainTextEdit *gloryOverview;

public:
    GloryDialog(BoardModel *boardModel, QWidget *parent = 0);
};

#endif // GLORYDIALOG_HPP
