#ifndef COMMON_HPP
#define COMMON_HPP

#include <QObject>
#include <QTime>

namespace Common
{
inline int random()
{
    return qrand();
}

inline void newTimeSeed()
{
    QTime current = QTime::currentTime();
    qsrand(current.hour() * current.minute() * current.second() * current.msec());
}

inline void newSeed(int seed)
{
    qsrand(seed);
    return;
}

inline int randomSeed()
{
    newTimeSeed();
    return random();
}

inline int randomSeed(int seed)
{
    newSeed(seed);
    return random();
}
}

#endif // COMMON_HPP
