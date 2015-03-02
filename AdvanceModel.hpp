#ifndef ADVANCEMODEL_H
#define ADVANCEMODEL_H

#include <QObject>

class AdvanceModel : public QObject
{
public:
    enum Advance
    {
        AGRICULTURE = 0,
        ARCHITECTURE,
        ARTS,
        ASTRONOMY,
        BANKING,
        BASIC_TOOLS,
        BLACK_MARKET,
        CARTAGE,
        CAVALRY,
        CENTRALIZED_GOVERNMENT,
        CIVIL_SERVICE,
        COMMON_TONGUE,
        COINAGE,
        CULTURE_OF_THIEVES,
        DEMOCRACY,
        DIPLOMACY,
        EQUESTRIAN,
        ENGINEERING,
        FISHING,
        GOVERNMENT,
        HORTICULTURE,
        IRRIGATION,
        LAW,
        LITERACY,
        MACHINING,
        MAGNETICS,
        MASONRY,
        MEDICINE,
        MEDITATION,
        METAL_WORKING,
        MILITARY,
        MINING,
        MINISTRY,
        MUSIC,
        MYTHOLOGY,
        NAVIGATION,
        ORGANIZED_RELIGION,
        PATRONAGE,
        PHILOSOPHY,
        ROADBUILDING,
        SAILS_AND_RIGGINGS,
        SENSE_OF_COMMUNITY,
        SHIPPING,
        SIMPLE_TOOLS,
        SLAVE_LABOR,
        STORY_TELLING,
        SURVEYING,
        THEATER,
        WRITTEN_RECORD
    };

    enum PrequisiteType{NONE = 0, SINGLE, OR, AND};

private:
    Advance advance;
    QString name;
    int victoryPoints;
    int tribesCost;
    int goldCost;
    bool requiresWood;
    bool requiresStone;
    bool requiresFood;
    QList<QList<Advance> > prequisites; // I.e.: A list [[A,B],[A,C]] means this advance needs (A AND B) OR (A AND C).
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
                 const QList<QList<Advance> > &prequisites,
                 const QList<QString> &positiveEffects,
                 const QList<QString> &negativeEffects,
                 QObject *parent = 0);

    // Get-Methods
    Advance getAdvance() const;
    QString getName() const;
    int getVictoryPoints() const;
    int getTribesCost() const;
    int getGoldCost() const;
    bool getRequiresWood() const;
    bool getRequiresStone() const;
    bool getRequiresFood() const;
    QList<QList<Advance> > getPrequisites() const;
    QList<QString> getPositiveEffects() const;
    QList<QString> getNegativeEffects() const;
    QString getEffects() const;
};

#endif // ADVANCEMODEL_H
