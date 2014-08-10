#include "UpkeepInstruction.hpp"

UpkeepInstruction::UpkeepInstruction(BoardModel *boardModel)
    : boardModel(boardModel)
{}

void UpkeepInstruction::initInstruction()
{
    this->boardModel->clearMessages();
    this->boardModel->sendMessage("Upkeep:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Decimate unsupported tribes.");
    this->boardModel->sendMessage("A region can support as much tribes as there are Mountains,");
    this->boardModel->sendMessage("Volcanoes, Farms, Forests and and City AV in that region added up.");
    this->boardModel->decimateUnsupportedTribes();
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Decimate all Gold.");
    this->boardModel->setGold(0);
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Advance City AV.");
    this->boardModel->sendMessage("To advance a city's AV, you have to do the following:");
    this->boardModel->sendMessage("Choose a city and decimate as much tribes from anywhere in the EMPIRE");
    this->boardModel->sendMessage("until the amount decimated is equal the current City AV plus 1.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press done.");
}
