#include "WonderModel.hpp"

WonderModel::WonderModel(WonderModel::Wonder wonder, int victoryPoints, int tribesCost, int goldCost, bool requiresWood, bool requiresStone, bool requiresFood, bool requiresCity, const QList<AdvanceModel::Advance> &prequisites, const QList<QString> &otherRequirements, const QList<QString> &positiveEffects, const QList<QString> &negativeEffects, QObject *parent)
    : QObject(parent),
      wonder(wonder),
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

QString WonderModel::listUpWonders(const QList<WonderModel::Wonder> &wonders)
{
    QString result = "None";

    if(wonders.count() > 1)
    {
        result = "";
        for(int i = 0; i < wonders.count() - 2; ++i)
        {
            result = result.append(QString("%1, ").arg(WonderModel::getName(wonders[i])));
        }

        result = result.append(QString("%1 and %2")
                                .arg(WonderModel::getName(wonders[wonders.count() - 2]))
                                .arg(WonderModel::getName(wonders.last())));
    }
    else if(wonders.count() == 1)
    {
        result = WonderModel::getName(wonders.first());
    }

    return result;
}

QString WonderModel::getName(WonderModel::Wonder wonder)
{
    switch(wonder)
    {
        case WonderModel::AMPHITHEATER: return "Amphitheater";
        case WonderModel::CITY_OF_ATLANTIS: return "City of Atlantis";
        case WonderModel::COLISEUM_OF_DUELS: return "Coliseum of Duels";
        case WonderModel::GIANT_GILDED_STATUE: return "Giant Gilded Statue";
        case WonderModel::GREAT_WALL_OF_SOLITUDE: return "Great Wall of Solitude";
        case WonderModel::HALL_OF_JUSTICE: return "Hall of Justice";
        case WonderModel::HANGING_GARDENS: return "Hanging Gardens";
        case WonderModel::HUGE_MONOLITH: return "Huge Monolith";
        case WonderModel::MOUNTAIN_CITADEL: return "Mountain Citadel";
        case WonderModel::PALACE_OF_THE_SENATE: return "Palace of the Senate";
    }

    return "UNKNOWN WONDER";
}

WonderModel::Wonder WonderModel::getWonder() const
{
    return this->wonder;
}

QString WonderModel::getName() const
{
    return WonderModel::getName(this->wonder);
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


