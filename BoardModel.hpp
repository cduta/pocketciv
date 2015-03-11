#ifndef BOARDMODEL_HPP
#define BOARDMODEL_HPP

#include <QList>
#include <QSet>
#include <QDataStream>

#include "HexModel.hpp"
#include "RegionModel.hpp"
#include "EventCard.hpp"
#include "AdvanceModel.hpp"

#include <boost/assert.hpp>

class BoardModel : public QObject
{
    Q_OBJECT

public:
    enum Empire {ATLANTEA = 0, FLOREN, GILDA, NORDIG};

signals:
    void boardUpdated();
    void boardCleared();
    void sendMessage(const QString &text);
    void clearMessages();
    void sendCardsLeftCount(int cardsLeftCount);
    void sendDoneText(const QString &text);
    void sendDialogClosed();
    void eraChanged(int era);
    void goldChanged(int gold);
    void gloryScoreChanged(int gloryScore);
    void advanceAquired(AdvanceModel::Advance advance);
    void changeAdvanceSelected(AdvanceModel::Advance advance);

private:
    QList<QList<HexModel *> > hexModels;            // Saved, Initialize with newBoard(width,height)

    QList<QSet<HexModel *> > seas;                  // Derived from hexModels after loading (groupSeas())
    QMap<int, QSet<HexModel *> > regionHexes;       // Derived from hexModels after loading (deriveRegionHexes())
    QMap<int, RegionModel *> regions;               // Saved
    QList<const EventCard *> eventCardsLeft;        // Saved, content derived from eventCards

    RegionModel *activeRegion;                      // Initialized
    QList<const EventCard *> eventCards;            // Initialized

    QMap<AdvanceModel::Advance, const AdvanceModel *> advances;   // Initialized
    QSet<AdvanceModel::Advance> advancesAquired;                  // Saved
    QSet<AdvanceModel::Advance> selectedAdvances;                 // Derived (Empty at first)

    // Save
    bool buildCity;
    bool buildFarm;
    bool expedition;
    bool aquireAdvances;
    bool buildWonder;
    bool collectTaxes;
    bool forestation;
    bool mining;
    bool doneEnabled;

    int gold;

    int era;
    int lastEra;

    int gloryScore;

    bool agricultureLeft;

    // Initialized
    const EventCard *originalCard;

public:
    BoardModel(int width = 20, int height = 10, QObject *parent = 0);
    ~BoardModel();

    bool toggleHexToRegion(int region, int x, int y);
    void enableAllHexes();
    void setUnsetHexesToSea();
    void setChoosingHexesDone();
    void enableRegionSelectableHexes();
    void groupSeas();
    void initialRegionModels();

    bool canMoveTribes(int fromRegion, int toRegion);

    void mergeAllMovedTribes();

    void increaseEra();
    void populationGrowth();
    void moveTribes(int fromRegion, int toRegion, int howMany);

    bool bordersOnFrontier(int region);
    bool bordersOnSea(int region);
    bool bordersOnDesert(int region);

    void decimateUnsupportedTribes();
    void decimateGold();
    void checkCitySupport();
    int checkCartageCitySupport();
    void decimateZeroAVCities();

    void selectAdvanceableRegions();

    void scoreSelectedAdvances();
    void toggleSelectAquiredAdvance(AdvanceModel::Advance advance);

    const EventCard *drawCard(bool tell = true);
    const EventCard *drawOriginalCard(bool tell = true);
    void discardDrawnCards();
    void reshuffleCards();
    void setSelectRegion(int region, bool select);
    void unselectAllRegions();
    void disableButtons();
    void enableMainPhaseButtons();
    void decimateAllSelectedTribes();
    void unselectAllSelectedTribes();
    void addGold(int gold);
    void removeGold(int gold);

private:
    void newBoard(int width, int height);
    void initializeBoard();
    bool checkRegionHexSet(const QSet<HexModel *> &regionHexSet);
    void deriveRegionHexes();
    void initializeCards();

public:
// Get-Methods
    int getWidth() const;
    int getHeight() const;
    QMap<int, QSet<HexModel *> > getRegionHexes() const;
    QMap<int, RegionModel *> getRegions() const;
    QMap<int, RegionModel *> getAdjacentRegions(int fromRegion) const;
    QMap<int, RegionModel *> getMountainRegions() const;
    QMap<int, RegionModel *> getForestRegions() const;
    int getMountainCount() const;
    int getForestCount() const;
    int getFarmCount() const;
    int getDesertCount() const;
    int getTribeCount() const;
    int getCityCount() const;
    int getCityAVCount() const;
    QMap<int, RegionModel *> getSelectedRegions() const;
    bool canBuildCity() const;
    bool hasCity() const;
    bool hasCapitolAssigned() const;
    bool canBuildFarm() const;
    bool canDoExpedition() const;
    bool canAquireAdvance() const;
    bool canBuildWonder() const;
    bool canCollectTaxes() const;
    bool canDoForestation() const;
    bool canDoMining() const;
    bool isDoneEnabled() const;
    int getEra() const;
    int getLastEra() const;
    bool isEndOfEra() const;
    int getAllMovedTribes() const;
    int getAllSelectedTribes() const;
    int getGold() const;
    int getGloryScore() const;
    QSet<AdvanceModel::Advance> getAdvancesSelected() const;
    int getAdvanceSelectionLimit() const;
    bool hasAquiredAdvanceSelected(AdvanceModel::Advance advance) const;
    bool hasAdvanceAquired(AdvanceModel::Advance advance) const;
    QSet<AdvanceModel::Advance> getAdvancesAquired() const;
    bool hasAgricultureLeft() const;

// Set-Methods
    void setActiveRegion(int region, bool isBad = true);
    void unsetActiveRegion();
    void setEra(int era);
    void setGold(int gold);
    void setGloryScore(int gloryScore);
    void setAdvanceAquired(AdvanceModel::Advance advance);
    void setAgricultureLeft(bool agricultureLeft);
    void setDoneButton(bool enabled);

// Ref-Methods
    HexModel *refHexModel(int x, int y);
    RegionModel *refRegionModel(int x, int y);
    RegionModel *refRegionModel(int region) const;
    RegionModel *refActiveRegion() const;
    RegionModel *refCapitolRegion() const;
    const EventCard *refOriginalCard() const;
    const AdvanceModel *refAdvanceModel(AdvanceModel::Advance advance) const;

public slots:
     void aquireAdvance(AdvanceModel::Advance advance);

private slots:
     void clearBoard();

public:
     // Serialization
     void serialize(QDataStream &writer) const;
     void deserialize(QDataStream &reader);
};

#endif // BOARDMODEL_HPP
