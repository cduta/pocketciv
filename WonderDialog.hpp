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
    QPushButton *   submitButton;

public:
    WonderDialog(BoardModel *boardModel, WonderDescription::WonderDescriptionType wonderDescriptionType = WonderDescription::WonderDescriptionType::OVERVIEW, int region = -1, QWidget *parent = 0);

    void setCompactSize();
    void setFullSize();

    void setSelectionTotal(int selectionTotal);
    QMap<WonderModel::Wonder, int> getSelectedWonders() const;

public slots:
    void toggleSize();

private slots:
    void updateWonderSelection(WonderModel::Wonder,int);
};

#endif // WONDERDIALOG_HPP
