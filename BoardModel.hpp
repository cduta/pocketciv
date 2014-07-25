#ifndef BOARDMODEL_HPP
#define BOARDMODEL_HPP

#include <QList>

#include "HexModel.hpp"

#include <boost/assert.hpp>

class BoardModel : public QObject
{
    Q_OBJECT

signals:
    void boardUpdated();
    void boardCleared();

private:
    QList<QList<HexModel *> > hexModels;

public:
    BoardModel(int width, int height, QObject *parent = 0)
        : QObject(parent)
    {
        this->newBoard(width, height);
    }

    ~BoardModel()
    {
        this->clearBoard();
    }

    void newBoard(int width, int height)
    {
        this->clearBoard();

        for(int x = 0; x < width; ++x)
        {
            this->hexModels.append(QList<HexModel *>());
            for(int y = 0; y < height; ++y)
            {
                this->hexModels[x].append(new HexModel(x,y, true, DRAW_NO_BORDER, this));
            }
        }

        emit this->boardUpdated();
        return;
    }

    void initializeBoard()
    {
        for(int x = 0; x < this->hexModels.size(); ++x)
        {
            for(int y = 0; y < this->hexModels[x].size(); ++y)
            {
                QList<QPair<int, HexModel *> > adjacendHexes;

                if(x%2 != 0)
                {
                    if(x-1 > 0 && y-1 > 0)
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_UPPER_LEFT_BORDER, this->hexModels[x-1][y-1]));
                    }

                    if(y-1 > 0)
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_UPPER_CENTER_BORDER, this->hexModels[x][y-1]));
                    }

                    if(x+1 < this->hexModels.size() && y-1 > 0)
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_UPPER_RIGHT_BORDER, this->hexModels[x+1][y-1]));
                    }

                    if(x-1 > 0)
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_LOWER_LEFT_BORDER, this->hexModels[x-1][y]));
                    }

                    if(y+1 < this->hexModels[x].size())
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_LOWER_CENTER_BORDER, this->hexModels[x][y+1]));
                    }

                    if(x+1 < this->hexModels.size())
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_LOWER_RIGHT_BORDER, this->hexModels[x+1][y]));
                    }
                }
                else
                {
                    if(x-1 > 0)
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_UPPER_LEFT_BORDER, this->hexModels[x-1][y]));
                    }

                    if(y-1 > 0)
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_UPPER_CENTER_BORDER, this->hexModels[x][y-1]));
                    }

                    if(x+1 < this->hexModels.size())
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_UPPER_RIGHT_BORDER, this->hexModels[x+1][y]));
                    }

                    if(x-1 > 0 && y+1 < this->hexModels[x].size())
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_LOWER_LEFT_BORDER, this->hexModels[x-1][y+1]));
                    }

                    if(y+1 < this->hexModels[x].size())
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_LOWER_CENTER_BORDER, this->hexModels[x][y+1]));
                    }

                    if(x+1 < this->hexModels.size() && y+1 < this->hexModels[x].size())
                    {
                        adjacendHexes.append(QPair<int, HexModel*>(DRAW_LOWER_RIGHT_BORDER, this->hexModels[x+1][y+1]));
                    }
                }

                this->hexModels[x][y]->setAdjacentHexes(adjacendHexes);
            }
        }

        this->enableAllAvailableHexes();

        return;
    }

    void enableAllAvailableHexes()
    {
        for(int x = 0; x < this->hexModels.size(); ++x)
        {
            if(!this->hexModels[x].isEmpty())
            {
                this->hexModels[x][0]->setEnable(false);
                this->hexModels[x][this->hexModels[x].size()-1]->setEnable(false);
            }
        }

        if(!this->hexModels.isEmpty())
        {
            for(int y = 1; y < this->hexModels[0].size()-1; ++y)
            {
                this->hexModels[0][y]->setEnable(false);
            }

            for(int y = 1; y < this->hexModels[this->hexModels.size()-1].size()-1; ++y)
            {
                this->hexModels[this->hexModels.size()-1][y]->setEnable(false);
            }
        }

        return;
    }

public:
// Get-Methods
    int getWidth()
    {
        return this->hexModels.size();
    }

    int getHeight()
    {
        if(this->hexModels.isEmpty())
        {
            return 0;
        }

        return this->hexModels.first().size();
    }

// Ref-Models
    HexModel *refHexModel(int x, int y)
    {
        assert(x >= 0 && x < this->hexModels.size());
        assert(y >= 0 && y < this->hexModels[x].size());

        return this->hexModels[x][y];
    }

private slots:
     void clearBoard()
     {
         foreach(QList<HexModel *> list, this->hexModels)
         {
             foreach(HexModel *hexModel, list)
             {
                 hexModel->deleteLater();
             }
         }

         this->hexModels.clear();

         emit this->boardCleared();
     }
};

#endif // BOARDMODEL_HPP
