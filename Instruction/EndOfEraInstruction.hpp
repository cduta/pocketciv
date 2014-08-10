#ifndef ENDOFERAINSTRUCTION_HPP
#define ENDOFERAINSTRUCTION_HPP

#include "Instruction/Instruction.hpp"
#include "BoardModel.hpp"

class EndOfEraInstruction : public Instruction
{
    BoardModel *boardModel;
    Instruction *interruptedInstruction;

public:
    EndOfEraInstruction(BoardModel *boardModel, Instruction *interruptedInstruction);

    virtual void initInstruction();

    virtual Instruction *triggerDone();
};

#endif // ENDOFERAINSTRUCTION_HPP
