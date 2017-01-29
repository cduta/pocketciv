#ifndef INTEGERTABLEITEM_HPP
#define INTEGERTABLEITEM_HPP

#include <QTableWidgetItem>

class IntegerTableItem : public QTableWidgetItem
{
public:
    IntegerTableItem(int integer);

    bool operator< (const QTableWidgetItem &other) const;
};

#endif // INTEGERTABLEITEM_HPP
