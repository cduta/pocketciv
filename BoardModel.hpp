#ifndef BOARDMODEL_HPP
#define BOARDMODEL_HPP

#include <QList>
#include <QSet>

#include "HexModel.hpp"

#include <boost/assert.hpp>

class BoardModel : public QObject
{
    Q_OBJECT

signals:
    void boardUpdated();
    void boardCleared();
    void sendMessage(const QString &text);

private:
    QList<QList<HexModel *> > hexModels;
    QMap<int, QSet<HexModel *> > regions;
    QList<QSet<HexModel *> > seas;

public:
    BoardModel(int width, int height, QObject *parent = 0);
    ~BoardModel();

    bool toggleHexToRegion(int region, int x, int y);
    void enableAllHexes();
    void setUnsetHexesToSea();
    void setChoosingHexesDone();
    void enableRegionSelectableHexes();
    void groupSeas();

private:
    void newBoard(int width, int height);
    void initializeBoard();
    bool checkRegionSet(const QSet<HexModel *> &regionSet);

public:
// Get-Methods
    int getWidth() const;
    int getHeight() const;
    QMap<int, QSet<HexModel *> > getRegions() const;

// Ref-Models
    HexModel *refHexModel(int x, int y);

private slots:
     void clearBoard();
};

#endif // BOARDMODEL_HPP
