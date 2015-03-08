#include "EndGameInstruction.hpp"

EndGameInstruction::EndGameInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void EndGameInstruction::initInstruction()
{
    this->boardModel->sendMessage("This is the End of the Game...");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("To start choosing, press Done.");
    return;
}

Instruction *EndGameInstruction::triggerDone()
{
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Adding all VPs of the Wonders to the final Glory Score.");
    // TODO: Add the Wonders to the final glory score.
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage(QString("Your final Glory Score is: %1 !").arg(this->boardModel->getGloryScore()));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("THE GAME IS OVER.");
    this->boardModel->disableButtons();
    return new NoInstruction();
}
