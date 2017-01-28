#ifndef WONDERSTABLE_HPP
#define WONDERSTABLE_HPP

#include <QDialog>

#include "BoardModel.hpp"
#include "WonderDescription.hpp"

class WondersTable : public QDialog
{
    Q_OBJECT

    BoardModel *boardModel;
    WonderDescription::WonderDescriptionType wonderDescriptionType;

public:
    WondersTable(BoardModel *boardModel, WonderDescription::WonderDescriptionType wonderDescriptionType, QWidget *parent = 0);
};

#endif // WONDERSTABLE_HPP
