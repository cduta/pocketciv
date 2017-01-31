#ifndef WONDERDESCRIPTION_HPP
#define WONDERDESCRIPTION_HPP

#include <QObject>
#include <QDialog>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>

#include "BoardModel.hpp"
#include "WonderModel.hpp"

class WonderDescription : public QObject
{
    Q_OBJECT

public:
    enum WonderDescriptionType { OVERVIEW = 0, REGION_OVERVIEW, SELECTION, BUILD };

signals:
    void selectionCountChanged(WonderModel::Wonder wonder, int selectionChanged);
    void wonderBuilt(WonderModel::Wonder wonder);

private:
    BoardModel *boardModel;
    WonderModel::Wonder wonder;
    int wonderSelection;
    int wonderCount;
    WonderDescription::WonderDescriptionType wonderDescriptionType;

    QDialog dialog;
    QGridLayout *layout;
    QPlainTextEdit *description;
    QPushButton *buildWonder;
    QSpinBox *   wonderSelectionSpinBox;
    QPushButton *submitSelection;
    QPushButton *cancel;

public:
    WonderDescription(BoardModel *boardModel, WonderModel::Wonder wonder, int wonderCount, WonderDescription::WonderDescriptionType wonderDescriptionType, QObject *parent = 0);

    void updateDescription();
    int getWonderCount() const;
    int getWonderSelection() const;
public slots:
    void show();

private slots:
    void buildWonderClicked();
    void wonderSelectionChanged();
};

#endif // WONDERDESCRIPTION_HPP
