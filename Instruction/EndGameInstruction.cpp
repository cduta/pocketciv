#include "EndGameInstruction.hpp"

EndGameInstruction::EndGameInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void EndGameInstruction::initInstruction()
{
    this->boardModel->sendMessage("The Game Ends...");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("To calculate your final Glory Score, choose as");
    this->boardModel->sendMessage("many advances as there are Tribes in the EMPIRE.");
    this->boardModel->sendMessage("Their VPs will be added to your final Glory Score.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("To start choosing, press Done.");
    return;
}

Instruction *EndGameInstruction::triggerDone()
{
    // Choose your advances.
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Adding all VPs of the Wonders to the final Glory Score.");
    // Add the Wonders to the final glory score.
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage(QString("Your final Glory Score is: %1 !").arg(this->boardModel->getGloryScore()));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("The game is over.");
    this->boardModel->disableButtons();
    return new NoInstruction();
}
