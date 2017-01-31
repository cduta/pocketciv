#ifndef WONDERDIALOG_HPP
#define WONDERDIALOG_HPP

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

#include "BoardModel.hpp"
#include "WondersTable.hpp"

class WonderDialog : public QDialog
{
    Q_OBJECT

    BoardModel *    boardModel;
    bool            isCompact;
    const QSize     WONDERS_DIALOG_SIZE_COMPACT;
    const QSize     WONDERS_DIALOG_SIZE_FULL;
    const QString   BUTTON_TEXT_COMPACT;
    const QString   BUTTON_TEXT_FULL;

    QGridLayout *   wonderLayout;
    WondersTable *  wondersTable;
    QPushButton *   resizeButton;

public:
    WonderDialog(BoardModel *boardModel, WonderDescription::WonderDescriptionType wonderDescriptionType = WonderDescription::WonderDescriptionType::OVERVIEW, int region = -1, QWidget *parent = 0);

    void setCompactSize();
    void setFullSize();

public slots:
    void toggleSize();
};

#endif // WONDERDIALOG_HPP
