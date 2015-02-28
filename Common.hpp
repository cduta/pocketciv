#ifndef COMMON_HPP
#define COMMON_HPP

#include <QObject>
#include <QTime>
#include <QString>
#include <QList>

#include "AdvanceModel.hpp"
#include "BoardModel.hpp"
#include "RegionModel.hpp"

namespace Common
{

inline QString getEmpireName(BoardModel::Empire empire)
{
    switch(empire)
    {
        case BoardModel::ATLANTEA: return "ATLANTEA"; break;
        case BoardModel::FLOREN  : return "FLOREN";   break;
        case BoardModel::GILDA   : return "GILDA";    break;
        case BoardModel::NORDIG  : return "NORDIG";   break;
        default: break;
    }

    return "NONE";
}

inline QString listUpRegions(const QList<RegionModel *> &list)
{
    if(list.size() == 0)
    {
        return QString("None");
    }

    if(list.size() == 1)
    {
        return QString::number(list[0]->getRegion());
    }

    QString result;

    for(int i = 0; i < list.size() - 2; ++i)
    {
        result.append(QString("%1, ").arg(list[i]->getRegion()));
    }

    return(QString("%1%2 and %3")
           .arg(result)
           .arg(list[list.size()-2]->getRegion())
           .arg(list[list.size()-1]->getRegion()));
}

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
