#ifndef PLACEINITIALTRIBES_HPP
#define PLACEINITIALTRIBES_HPP

#include "Instruction/Instruction.hpp"

class PlaceInitialTribes : public Instruction
{
    Q_OBJECT

public:
    PlaceInitialTribes(BoardModel *boardModel, QObject *parent = 0);

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // PLACEINITIALTRIBES_HPP
