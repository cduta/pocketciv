#ifndef HEXMODEL_HPP
#define HEXMODEL_HPP

#include <QObject>
#include <QPair>

static const int DRAW_NO_BORDER           = 0;
static const int DRAW_UPPER_LEFT_BORDER   = 1;
static const int DRAW_UPPER_CENTER_BORDER = 2;
static const int DRAW_UPPER_RIGHT_BORDER  = 4;
static const int DRAW_LOWER_RIGHT_BORDER  = 8;
static const int DRAW_LOWER_CENTER_BORDER = 16;
static const int DRAW_LOWER_LEFT_BORDER   = 32;

class HexModel : public QObject
{
    Q_OBJECT

signals:
    void hexModelUpdated(int x, int y);

private:
    int xPos;
    int yPos;
    bool enable;
    int visibleBorders;
    QList<QPair<int, HexModel *> > adjacentHexes;

public:
    HexModel(int xPos = -1, int yPos = -1, bool enable = true, int visibleBorders = DRAW_NO_BORDER, QObject *parent = 0);

    void addVisibleBorders(int visibleBorders);
    void removeVisibleBorders(int visibleBorders);

    void updateHex();

public:
    // Get-Methods
    bool isEnabled() const;
    int getVisibleBorders() const;
    int x() const;
    int y() const;

    // Set-Methods
    void setEnable(bool enable);
    void setVisibleBorders(int visibleBorders);
    // Has to be at most 6 hexes.
    void setAdjacentHexes(QList<QPair<int, HexModel *> > &adjacentHexes);

private:
    // Return the draw border constant of this hex. Remember: You can convert it to the opposite side with oppositeBorder().
    int borderAdjacentTo(HexModel *hexModel) const;

    void privateAddVisibleBorders(int visibleBorders);
    void privateRemoveVisibleBorders(int visibleBorders);
};

inline int oppositeBorder(int visibleBorder)
{
    int result = 0;

    switch(visibleBorder)
    {
        case DRAW_UPPER_LEFT_BORDER: result = DRAW_LOWER_RIGHT_BORDER; break;
        case DRAW_UPPER_CENTER_BORDER: result = DRAW_LOWER_CENTER_BORDER; break;
        case DRAW_UPPER_RIGHT_BORDER: result = DRAW_LOWER_LEFT_BORDER; break;
        case DRAW_LOWER_RIGHT_BORDER: result = DRAW_UPPER_LEFT_BORDER; break;
        case DRAW_LOWER_CENTER_BORDER: result = DRAW_UPPER_CENTER_BORDER; break;
        case DRAW_LOWER_LEFT_BORDER: result = DRAW_UPPER_RIGHT_BORDER; break;
    }

    return result;
}

#endif // HEXMODEL_HPP
