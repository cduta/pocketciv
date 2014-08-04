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

private:
    QList<QList<HexModel *> > hexModels;
    QMap<int, QSet<HexModel *> > regionHexes;
    QList<QSet<HexModel *> > seas;
    QMap<int, RegionModel *> regions;
    QSet<const EventCard *> eventCards;
    QList<const EventCard *> eventCardsLeft;

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

    void populationGrowth();

    const EventCard *drawCard();

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

// Ref-Models
    HexModel *refHexModel(int x, int y);
    RegionModel *refRegionModel(int x, int y);
    RegionModel *refRegionModel(int region);

private slots:
     void clearBoard();
};

#endif // BOARDMODEL_HPP
