#ifndef PLACEINITIALTRIBES_HPP
#define PLACEINITIALTRIBES_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class PlaceInitialTribes : public Instruction
{
    BoardModel *boardModel;

public:
    PlaceInitialTribes(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
};

#endif // PLACEINITIALTRIBES_HPP
