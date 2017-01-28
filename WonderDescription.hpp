#ifndef WONDERDESCRIPTION_HPP
#define WONDERDESCRIPTION_HPP

#include <QObject>
#include <QDialog>

#include "BoardModel.hpp"

class WonderDescription : public QObject
{
    Q_OBJECT

public:
    enum WonderDescriptionType { OVERVIEW = 0, REGION_OVERVIEW, SELECTION, BUILD };

public:
    WonderDescription(BoardModel *boardModel, WonderDescription::WonderDescriptionType wonderDescriptionType, QObject *parent = 0);
};

#endif // WONDERDESCRIPTION_HPP
