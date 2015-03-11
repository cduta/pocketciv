#include "UpkeepInstruction.hpp"

#include "Instruction/PopulationGrowthInstruction.hpp"
#include "Instruction/EndGameInstruction.hpp"

#include "Instruction/DecimateGoldInstruction.hpp"

UpkeepInstruction::UpkeepInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void UpkeepInstruction::initInstruction()
{
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("UPKEEP:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Decimate unsupported tribes.");
    this->boardModel->sendMessage("A region can support as much tribes as there are Mountains,");
    this->boardModel->sendMessage("Volcanoes, Farms, Forests and City AV in that region added up.");
    this->boardModel->decimateUnsupportedTribes();
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue...");
    this->boardModel->sendMessage(" ");
}

Instruction *UpkeepInstruction::triggerDone()
{
    Instruction *next = new DecimateGoldInstruction(this->boardModel);
    next->initInstruction();
    return next;
}

