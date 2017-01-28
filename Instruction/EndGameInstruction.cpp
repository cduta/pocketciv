#include "EndGameInstruction.hpp"

EndGameInstruction::EndGameInstruction(BoardModel *boardModel, bool countAdvances)
    : Instruction(), boardModel(boardModel), countAdvances(countAdvances)
{}

void EndGameInstruction::initInstruction()
{
    this->boardModel->printMessage("This is the End of the Game...");

    this->boardModel->printMessage(" ");
    if(this->countAdvances)
    {
        this->boardModel->addAdvanceGloryScore();
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("Now add all VPs of the Wonders to the final Glory Score.");
    int wonderVP = 0;

    QMap<WonderModel::Wonder, int> builtWonders = this->boardModel->getAllBuiltWonders();

    foreach(int wvp, builtWonders.values())
    {
        wonderVP += wvp;
    }

    this->boardModel->setGloryScore(this->boardModel->getGloryScore() + wonderVP);

    this->boardModel->printMessage(QString("Your VPs from Wonders is %1.").arg(wonderVP));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage(QString("Your final Glory Score is: %1 !").arg(this->boardModel->getGloryScore()));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("THE GAME IS OVER.");
    this->boardModel->disableButtons();
    return;
}

Instruction *EndGameInstruction::triggerDone()
{
    return new NoInstruction();
}
