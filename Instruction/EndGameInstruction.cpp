#include "EndGameInstruction.hpp"

EndGameInstruction::EndGameInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void EndGameInstruction::initInstruction()
{
    this->boardModel->printMessage("This is the End of the Game...");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("To start choosing, press Done.");
    return;
}

Instruction *EndGameInstruction::triggerDone()
{
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Adding all VPs of the Wonders to the final Glory Score.");
    // TODO: Add the Wonders to the final glory score.
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage(QString("Your final Glory Score is: %1 !").arg(this->boardModel->getGloryScore()));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("THE GAME IS OVER.");
    this->boardModel->disableButtons();
    return new NoInstruction();
}
