#ifndef MAINPHASEINSTRUCTION_HPP
#define MAINPHASEINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"
#include "WonderDialog.hpp"

class MainPhaseInstruction : public Instruction
{
    Q_OBJECT

    BoardModel *boardModel;
    bool wonderDialogOpen;
    WonderDialog *wonderDialog;

public:
    MainPhaseInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();
    virtual Instruction *triggerSaveGame();

private slots:
    void doneWonderDialogOverview();
};

#endif // MAINPHASEINSTRUCTION_HPP
