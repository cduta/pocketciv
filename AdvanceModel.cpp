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
