#ifndef ENDOFERAINSTRUCTION_HPP
#define ENDOFERAINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"
#include "AdvancesDialog.hpp"

class EndOfEraInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *interruptedInstruction;
    bool endGame;
    bool advancesChosen;

    AdvancesDialog advancesDialog;

public:
    EndOfEraInstruction(BoardModel *boardModel, Instruction *interruptedInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerDone();

private slots:
    void doneSelectingAdvances();
};

#endif // ENDOFERAINSTRUCTION_HPP
