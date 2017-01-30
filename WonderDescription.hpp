#ifndef WONDERDESCRIPTION_HPP
#define WONDERDESCRIPTION_HPP

#include <QObject>
#include <QDialog>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QPushButton>

#include "BoardModel.hpp"
#include "WonderModel.hpp"

class WonderDescription : public QObject
{
    Q_OBJECT

public:
    enum WonderDescriptionType { OVERVIEW = 0, REGION_OVERVIEW, SELECTION, BUILD };

signals:
    void wonderBuilt(WonderModel::Wonder wonder);

private:
    BoardModel *boardModel;
    WonderModel::Wonder wonder;
    WonderDescription::WonderDescriptionType wonderDescriptionType;

    QDialog dialog;
    QGridLayout *layout;
    QPlainTextEdit *description;
    QPushButton *buildWonder;
    QPushButton *cancel;

public:
    WonderDescription(BoardModel *boardModel, WonderModel::Wonder wonder, WonderDescription::WonderDescriptionType wonderDescriptionType, QObject *parent = 0);

    void updateDescription();
public slots:
    void show();

private slots:
    void buildWonderClicked();
};

#endif // WONDERDESCRIPTION_HPP
