#include "EndOfEraInstruction.hpp"

EndOfEraInstruction::EndOfEraInstruction(BoardModel *boardModel, Instruction *interruptedInstruction)
    : Instruction(), boardModel(boardModel), interruptedInstruction(interruptedInstruction)
{
    this->interruptedInstruction->setKeepInstruction(true);
}

void EndOfEraInstruction::initInstruction()
{
    this->boardModel->sendMessage("END OF ERA");
    this->boardModel->sendMessage(" ");
    // Check if city amount is >= current era.
    // Choose as many advances, as there are tribes in the empire (new instruction and return).
    // otherwise continue:
    // Choose if you want to end the game.
    this->boardModel->sendMessage("When you are done, press Done...");
    return;
}

Instruction *EndOfEraInstruction::triggerDone()
{
    if(this->boardModel->getEra() == this->boardModel->getLastEra())
    {
        this->boardModel->sendMessage("The last era is over...");
        this->boardModel->sendMessage("The game ends...");
        //this->nextInstruction->deleteLater();
        //set end of game instruction.
    }
    else
    {
        this->boardModel->sendMessage("The era increases by 1.");
        this->boardModel->increaseEra();
        this->boardModel->sendMessage("Reshuffle cards...");
        this->boardModel->reshuffleCards();
        this->boardModel->sendMessage("Discarding 3 event cards...");
        this->boardModel->drawCard(false);
        this->boardModel->drawCard(false);
        this->boardModel->drawCard(false);
    }

    this->boardModel->sendMessage(" ");
    this->interruptedInstruction->setKeepInstruction(false);
    return this->interruptedInstruction;
}

