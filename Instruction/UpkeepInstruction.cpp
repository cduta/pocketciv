#include "UpkeepInstruction.hpp"

#include "Instruction/PopulationGrowthInstruction.hpp"
#include "Instruction/EndGameInstruction.hpp"

#include "Instruction/DecimateGoldInstruction.hpp"

UpkeepInstruction::UpkeepInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void UpkeepInstruction::initInstruction()
{
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("UPKEEP:");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Decimate unsupported tribes.");
    this->boardModel->printMessage("A region can support as much tribes as there are Mountains,");
    this->boardModel->printMessage("Volcanoes, Farms, Forests and City AV in that region added up.");
    this->boardModel->decimateUnsupportedTribes();
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to continue...");
    this->boardModel->printMessage(" ");
}

Instruction *UpkeepInstruction::triggerDone()
{
    Instruction *next = new DecimateGoldInstruction(this->boardModel);
    next->initInstruction();
    return next;
}

