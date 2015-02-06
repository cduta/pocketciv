#ifndef MAINPHASEINSTRUCTION_HPP
#define MAINPHASEINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class MainPhaseInstruction : public Instruction
{
    BoardModel *boardModel;
    bool initialized;
    bool buildCity;
    bool buildFarm;
    bool expedition;
    bool expeditionCardDrawn;
    int gainGold;

public:
    MainPhaseInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

    virtual Instruction *triggerBuildCity();
    virtual Instruction *triggerBuildFarm();
    virtual Instruction *triggerExpedition();
    virtual Instruction *triggerAquireAdvance();
    virtual Instruction *triggerBuildWonder();

    virtual Instruction *triggerSaveGame();
};

#endif // MAINPHASEINSTRUCTION_HPP
