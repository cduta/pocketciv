#ifndef BOARDMODEL_HPP
#define BOARDMODEL_HPP

#include <QList>
#include <QSet>

#include "HexModel.hpp"
#include "RegionModel.hpp"
#include "EventCard.hpp"

#include <boost/assert.hpp>

class BoardModel : public QObject
{
    Q_OBJECT

signals:
    void boardUpdated();
    void boardCleared();
    void sendMessage(const QString &text);
    void clearMessages();
    void sendCardsLeftCount(int cardsLeftCount);
    void sendDoneText(const QString &text);
    void sendDialogClosed();
    void goldChanged(int gold);
    void gloryScoreChanged(int gloryScore);

private:
    QList<QList<HexModel *> > hexModels;
    QList<QSet<HexModel *> > seas;
    QMap<int, QSet<HexModel *> > regionHexes;
    QMap<int, RegionModel *> regions;
    RegionModel *activeRegion;
    QSet<const EventCard *> eventCards;
    QList<const EventCard *> eventCardsLeft;
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

    const EventCard *originalCard;

public:
    BoardModel(int width, int height, QObject *parent = 0);
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
    void decimateUnsupportedTribes();
    void decimateGold();
    void checkCitySupport();
    void decimateZeroAVCities();

    const EventCard *drawCard(bool tell = true);
    const EventCard *drawOriginalCard(bool tell = true);
    void discardDrawnCards();
    void reshuffleCards();
    void setSelectRegion(int region, bool select);
    void unselectAllRegions();
    void disableButtons();
    void enableDoneButton();
    void enableMainPhaseButtons();
    void decimateAllSelectedTribes();
    void unselectAllSelectedTribes();

private:
    void newBoard(int width, int height);
    void initializeBoard();
    bool checkRegionHexSet(const QSet<HexModel *> &regionHexSet);

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
    int getDesertCount() const;
    int getTribeCount() const;
    QMap<int, RegionModel *> getSelectedRegions() const;
    bool canBuildCity() const;
    bool hasCity() const;
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
    int getGloryScore() const;

// Set-Methods
    void setActiveRegion(int region);
    void unsetActiveRegion();
    void setGold(int gold);
    void setGloryScore(int gloryScore);

// Ref-Methods
    HexModel *refHexModel(int x, int y);
    RegionModel *refRegionModel(int x, int y);
    RegionModel *refRegionModel(int region) const;
    RegionModel *refActiveRegion() const;
    const EventCard *refOriginalCard() const;

private slots:
     void clearBoard();
};

#endif // BOARDMODEL_HPP
