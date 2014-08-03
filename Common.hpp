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

inline void randomSeed()
{
    QTime current = QTime::currentTime();
    qsrand(current.hour() * current.minute() * current.second() * current.msec());
}

inline void randomSeed(int seed)
{
    qsrand(seed);
    return;
}

inline int randomWithSeed()
{
    randomSeed();
    return random();
}

inline int randomWithSeed(int seed)
{
    randomSeed(seed);
    return random();
}
}

#endif // COMMON_HPP
