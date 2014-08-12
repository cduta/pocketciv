#include "MainPhaseInstruction.hpp"

#include "UpkeepInstruction.hpp"

MainPhaseInstruction::MainPhaseInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel), initialized(false)
{}

void MainPhaseInstruction::initInstruction()
{
    this->boardModel->sendMessage("Main Phase:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done, to begin.");
    return;
}

Instruction *MainPhaseInstruction::triggerDone()
{
    if(!this->initialized)
    {
        this->boardModel->clearMessages();
        this->boardModel->sendMessage("Main Phase:");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("You can do any amount of actions by clicking on any of the buttons.");
        this->boardModel->sendMessage("If clicked once, the action is first explained before applying it.");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("When you are Done, press Done.");
        this->boardModel->sendMessage(" ");
        this->boardModel->enableMainPhaseButtons();
        this->initialized = true;
        return this;
    }

    if(this->initialized)
    {
        this->boardModel->disableButtons();
        this->boardModel->enableDoneButton();
        Instruction *next = new UpkeepInstruction(this->boardModel);
        next->initInstruction();
        return next;
    }

    return this;
}
