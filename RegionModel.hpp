#ifndef REGIONMODEL_HPP
#define REGIONMODEL_HPP

#include <QObject>

class RegionModel : public QObject
{
    Q_OBJECT
    int tribes;
    bool mountain;
    bool forest;
    bool desert;

public:
    RegionModel(QObject *parent = 0);

public:
    void toggleMountain();
    void toggleForest();

    // Get-Methods
    int getTribes() const;
    bool hasMountain() const;
    bool hasForest() const;
    bool hasDesert() const;

    // Set-Methods
    void setTribes(int tribes);
    void setDesert();
};

#endif // REGIONMODEL_HPP
