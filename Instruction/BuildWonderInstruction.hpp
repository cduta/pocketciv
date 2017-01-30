#ifndef BUILDWONDERINSTRUCTION_HPP
#define BUILDWONDERINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"
#include "WonderDialog.hpp"

class BuildWonderInstruction : public Instruction
{
    Q_OBJECT

    BoardModel *boardModel;
    Instruction *nextInstruction;
    WonderDialog *wonderDialog;

public:
    BuildWonderInstruction(BoardModel *boardModel, Instruction *nextInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private slots:
    void doneBuildingWonders();
};

#endif // BUILDWONDERINSTRUCTION_HPP
