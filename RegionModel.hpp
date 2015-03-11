#ifndef REGIONMODEL_HPP
#define REGIONMODEL_HPP

#include <QObject>

class RegionModel : public QObject
{
    Q_OBJECT

    // Save
    int region;
    int tribes;
    int movedTribes;
    int selectedTribes;
    bool mountain;
    bool volcano;
    bool forest;
    bool farm;
    bool desert;
    bool faultLine;
    bool selected;
    bool city;
    bool capitol;
    int cityAV;
    bool advanceAquired;

public:
    RegionModel(int region = -1, QObject *parent = 0);

public:
    int getTribeSupportCount() const;

    void toggleMountain();
    void toggleForest();

    void addToMovedTribes(int movedTribes);
    void removeFromMovedTribes(int movedTribes);
    void mergeMovedTribes();

    void decimateTribes(int tribes);
    void decimateSelectedTribes();

    void increaseCityAV(int cityAV);
    void decreaseCityAV(int cityAV);
    void decimateZeroAVCity();

    void decimateWonders();

    // Get-Methods
    int getRegion() const;
    int getTribes() const;
    int getAvailableTribes() const;
    bool hasTribes() const;
    int getMovedTribes() const;
    int getSelectedTribes() const;
    bool hasMountain() const;
    bool hasVolcano() const;
    bool hasForest() const;
    bool hasFarm() const;
    bool hasDesert() const;
    bool hasFaultLine() const;
    bool isSelected() const;
    bool hasCity() const;
    bool isCapitolRegion() const;
    int getCityAV() const;
    bool hasAdvanceAquired() const;

    // Set-Methods
    void setTribes(int tribes);
    void setCity(bool city);
    void setCapitolRegion(bool capitol);
    void toggleCapitol();
    void setCityAV(int cityAV);
    void setSelectedTribes(int selectedTribes);
    void setMountain(bool mountain);
    void setVolcano(bool volcano);
    void setForest(bool forest);
    void setFarm(bool farm);
    void setDesert(bool desert);
    void setFaultLine(bool faultLine);
    void setSelected(bool selected);
    void setAdvanceAquired(bool advanceAquired);

public:
     // Serialization
     void serialize(QDataStream &writer) const;
     void deserialize(QDataStream &reader);
};

#endif // REGIONMODEL_HPP
