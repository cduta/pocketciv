#ifndef DECISIONDIALOG_HPP
#define DECISIONDIALOG_HPP

#include <QDialog>
#include <QString>
#include <QGridLayout>

#include "BoardModel.hpp"

class DecisionDialog : public QDialog
{
    Q_OBJECT

    BoardModel *boardModel;

public:
    DecisionDialog(BoardModel *boardModel,
                   const QString &title,
                   const QString &text,
                   const QString &acceptButtonText,
                   const QString &rejectButtonText,
                   bool showOverview = false);

private slots:
    void showOverview();
};

#endif // DECISIONDIALOG_HPP
