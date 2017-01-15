#include "AdvanceModel.hpp"

AdvanceModel::AdvanceModel(Advance advance,
                           const QString &name,
                           int victoryPoints,
                           int tribesCost,
                           int goldCost,
                           bool requiresWood,
                           bool requiresStone,
                           bool requiresFood,
                           const QList<QList<Advance> > &prequisites,
                           const QList<QString> &positiveEffects,
                           const QList<QString> &negativeEffects,
                           QObject *parent)
    : QObject(parent),
      advance(advance),
      name(name),
      victoryPoints(victoryPoints),
      tribesCost(tribesCost),
      goldCost(goldCost),
      requiresWood(requiresWood),
      requiresStone(requiresStone),
      requiresFood(requiresFood),
      prequisites(prequisites),
      positiveEffects(positiveEffects),
      negativeEffects(negativeEffects)
{}

bool AdvanceModel::advanceRequirementsMet(const QSet<Advance> &advancesAquired) const
{
    if(this->prequisites.isEmpty())
    {
        return true;
    }

    bool result = true;

    foreach(QList<Advance> andPart, this->prequisites)
    {
        foreach(Advance advancePrequisite, andPart)
        {
            result = result && advancesAquired.contains(advancePrequisite);
        }

        if(result)
        {
            return true;
        }

        result = true;
    }

    return false;
}

bool AdvanceModel::hasStoryTellingDiscount(AdvanceModel::Advance advance)
{
    return advance == Advance::MYTHOLOGY || advance == Advance::MUSIC ||
           advance == Advance::LITERACY || advance == Advance::PHILOSOPHY ||
           advance == Advance::ARTS || advance == Advance::THEATER ||
           advance == Advance::ORGANIZED_RELIGION || advance == Advance::LAW;
}

AdvanceModel::Advance AdvanceModel::getAdvance() const
{
    return this->advance;
}

QString AdvanceModel::getName() const
{
    return this->name;
}

int AdvanceModel::getVictoryPoints() const
{
    return this->victoryPoints;
}

int AdvanceModel::getTribesCost() const
{
    return this->tribesCost;
}

int AdvanceModel::getGoldCost() const
{
    return this->goldCost;
}

bool AdvanceModel::getRequiresWood() const
{
    return this->requiresWood;
}

bool AdvanceModel::getRequiresStone() const
{
    return this->requiresStone;
}

bool AdvanceModel::getRequiresFood() const
{
    return this->requiresFood;
}

QList<QList<AdvanceModel::Advance> > AdvanceModel::getPrequisites() const
{
    return this->prequisites;
}

QList<QString> AdvanceModel::getPositiveEffects() const
{
    return this->positiveEffects;
}

QList<QString> AdvanceModel::getNegativeEffects() const
{
    return this->negativeEffects;
}

QString AdvanceModel::getEffects() const
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
