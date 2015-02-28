#ifndef ADVANCEMODEL_H
#define ADVANCEMODEL_H

#include <QObject>

class AdvanceModel : public QObject
{
public:
    enum Advance
    {
        AGRICULTURE = 0
    };

private:
    Advance advance;
    QString name;
    int victoryPoints;
    int tribesCost;
    int goldCost;
    bool requiresWood;
    bool requiresStone;
    bool requiresFood;
    QList<QString> positiveEffects;
    QList<QString> negativeEffects;

public:
    AdvanceModel(Advance advance,
                 const QString &name,
                 int victoryPoints,
                 int tribesCost,
                 int goldCost,
                 bool requiresWood,
                 bool requiresStone,
                 bool requiresFood,
                 const QList<QString> &positiveEffects,
                 const QList<QString> &negativeEffects);
};

#endif // ADVANCEMODEL_H
