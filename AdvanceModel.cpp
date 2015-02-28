#include "AdvanceModel.hpp"

AdvanceModel::AdvanceModel(Advance advance,
                           const QString &name,
                           int victoryPoints,
                           int tribesCost,
                           int goldCost,
                           bool requiresWood,
                           bool requiresStone,
                           bool requiresFood,
                           const QList<QString> &positiveEffects,
                           const QList<QString> &negativeEffects)
    : advance(advance),
      name(name),
      victoryPoints(victoryPoints),
      tribesCost(tribesCost),
      goldCost(goldCost),
      requiresWood(requiresWood),
      requiresStone(requiresStone),
      requiresFood(requiresFood),
      positiveEffects(positiveEffects),
      negativeEffects(negativeEffects)
{}
