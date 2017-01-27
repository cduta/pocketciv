#ifndef WONDERMODEL_HPP
#define WONDERMODEL_HPP

#include <QObject>
#include <QList>
#include <QString>

#include "AdvanceModel.hpp"

class WonderModel : public QObject
{
public:
    enum Wonder
    {
        AMPHITHEATER = 0,
        CITY_OF_ATLANTIS,
        COLISEUM_OF_DUELS,
        GIANT_GILDED_STATUE,
        GREAT_WALL_OF_SOLITUDE,
        HALL_OF_JUSTICE,
        HANGING_GARDENS,
        HUGE_MONOLITH,
        MOUNTAIN_CITADEL,
        PALACE_OF_THE_SENATE
    };

private:
    Wonder wonder;
    QString name;
    int victoryPoints;
    int tribesCost;
    int goldCost;
    bool requiresWood;
    bool requiresStone;
    bool requiresFood;
    bool requiresCity;
    QList<AdvanceModel::Advance> advancePrequisites;
    QList<QString> otherRequirements;
    QList<QString> positiveEffects;
    QList<QString> negativeEffects;

public:
    WonderModel(Wonder wonder,
                 const QString &name,
                 int victoryPoints,
                 int tribesCost,
                 int goldCost,
                 bool requiresWood,
                 bool requiresStone,
                 bool requiresFood,
                 bool requiresCity,
                 const QList<AdvanceModel::Advance> &advancePrequisites,
                 const QList<QString> &otherRequirements,
                 const QList<QString> &positiveEffects,
                 const QList<QString> &negativeEffects,
                 QObject *parent = 0);

    bool advancePrequisitesMet(const QSet<AdvanceModel::Advance> &advancesAquired) const;

    // Get-Methods
    Wonder getWonder() const;
    QString getName() const;
    int getVictoryPoints() const;
    int getTribesCost() const;
    int getGoldCost() const;
    bool getRequiresWood() const;
    bool getRequiresStone() const;
    bool getRequiresFood() const;
    bool getRequiresCity() const;
    QList<AdvanceModel::Advance> getAdvancePrequisites() const;
    QList<QString> getOtherRequirements() const;
    QList<QString> getPositiveEffects() const;
    QList<QString> getNegativeEffects() const;
    QString getEffects() const;
};

#endif // WONDERMODEL_HPP
