#include "IntegerTableItem.hpp"

IntegerTableItem::IntegerTableItem(int integer)
    : QTableWidgetItem(QString::number(integer))
{
}

bool IntegerTableItem::operator<(const QTableWidgetItem &other) const
{
    return text().toInt() < other.text().toInt();
}
