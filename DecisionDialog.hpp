#ifndef DECISIONDIALOG_HPP
#define DECISIONDIALOG_HPP

#include <QDialog>
#include <QString>
#include <QGridLayout>

class DecisionDialog : public QDialog
{
    Q_OBJECT

signals:
    void showOverview();

public:
    DecisionDialog(const QString &title,
                   const QString &text,
                   const QString &acceptButtonText,
                   const QString &rejectButtonText,
                   bool showOverview = false);
};

#endif // DECISIONDIALOG_HPP
