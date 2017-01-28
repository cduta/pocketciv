#include "WondersTable.hpp"

WondersTable::WondersTable(BoardModel *boardModel, WonderDescription::WonderDescriptionType wonderDescriptionType, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel),
      wonderDescriptionType(wonderDescriptionType)
{

}
