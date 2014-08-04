#ifndef EVENTCARD_HPP
#define EVENTCARD_HPP

#include <QObject>

class EventCard : public QObject
{
    Q_OBJECT

public:
    EventCard(QObject *parent = 0);
};

#endif // EVENTCARD_HPP
