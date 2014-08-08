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

private:
    QList<QList<HexModel *> > hexModels;
    QList<QSet<HexModel *> > seas;
    QMap<int, QSet<HexModel *> > regionHexes;
    QMap<int, RegionModel *> regions;
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

    int era;
    int lastEra;

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

    void mergeTribesAllRegions();

    void populationGrowth();
    void moveTribes(int fromRegion, int toRegion, int howMany);

    const EventCard *drawCard();
    const EventCard *drawEventCard();
    void setSelectRegion(int region, bool select);
    void unselectAllRegions();
    void disableButtons();
    void enableButtons();

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
    QMap<int, RegionModel *> getMountainRegions() const;
    QMap<int, RegionModel *> getForestRegions() const;
    int getMountainCount() const;
    int getForestCount() const;
    int getDesertCount() const;
    int getTribeCount() const;
    QMap<int, RegionModel *> getSelectedRegions() const;
    bool canBuildCity() const;
    bool canBuildFarm() const;
    bool canDoExpedition() const;
    bool canAquireAdvance() const;
    bool canBuildWonder() const;
    bool canCollectTaxes() const;
    bool canDoForestation() const;
    bool canDoMining() const;
    bool isDoneEnabled() const;
    int getEra();
    int getLastEra();

// Ref-Methods
    HexModel *refHexModel(int x, int y);
    RegionModel *refRegionModel(int x, int y);
    RegionModel *refRegionModel(int region);

private slots:
     void clearBoard();
};

#endif // BOARDMODEL_HPP
