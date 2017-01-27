#include "WonderModel.hpp"

WonderModel::WonderModel(WonderModel::Wonder wonder, const QString &name, int victoryPoints, int tribesCost, int goldCost, bool requiresWood, bool requiresStone, bool requiresFood, bool requiresCity, const QList<AdvanceModel::Advance> &prequisites, const QList<QString> &otherRequirements, const QList<QString> &positiveEffects, const QList<QString> &negativeEffects, QObject *parent)
    : QObject(parent),
      wonder(wonder),
      name(name),
      victoryPoints(victoryPoints),
      tribesCost(tribesCost),
      goldCost(goldCost),
      requiresWood(requiresWood),
      requiresStone(requiresStone),
      requiresFood(requiresFood),
      requiresCity(requiresCity),
      advancePrequisites(prequisites),
      otherRequirements(otherRequirements),
      positiveEffects(positiveEffects),
      negativeEffects(negativeEffects)
{

}

bool WonderModel::advancePrequisitesMet(const QSet<AdvanceModel::Advance> &advancesAquired) const
{
    bool result = true;

    foreach(AdvanceModel::Advance advancePrequisite, this->advancePrequisites)
    {
        result = result && advancesAquired.contains(advancePrequisite);
    }

    return result;
}

WonderModel::Wonder WonderModel::getWonder() const
{
    return this->wonder;
}

QString WonderModel::getName() const
{
    return this->name;
}

int WonderModel::getVictoryPoints() const
{
    return this->victoryPoints;
}

int WonderModel::getTribesCost() const
{
    return this->tribesCost;
}

int WonderModel::getGoldCost() const
{
    return this->goldCost;
}

bool WonderModel::getRequiresWood() const
{
    return this->requiresWood;
}

bool WonderModel::getRequiresStone() const
{
    return this->requiresStone;
}

bool WonderModel::getRequiresFood() const
{
    return this->requiresFood;
}

bool WonderModel::getRequiresCity() const
{
    return this->requiresCity;
}

QList<AdvanceModel::Advance> WonderModel::getAdvancePrequisites() const
{
    return this->advancePrequisites;
}

QList<QString> WonderModel::getOtherRequirements() const
{
    return this->otherRequirements;
}

QList<QString> WonderModel::getPositiveEffects() const
{
    return this->positiveEffects;
}

QList<QString> WonderModel::getNegativeEffects() const
{
    return this->negativeEffects;
}

QString WonderModel::getEffects() const
{
    QString result;

    foreach(QString positive, this->positiveEffects)
    {
        result.append(positive).append("\n");
    }

    foreach(QString negative, this->negativeEffects)
    {
        result.append(negative).append("\n");
    }

    return result;
}


