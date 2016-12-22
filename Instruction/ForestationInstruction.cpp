#include "ForestationInstruction.hpp"

ForestationInstruction::ForestationInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : boardModel(boardModel), nextInstruction(nextInstruction), tribesCost(4)
{
    this->nextInstruction->setKeepInstruction(true);
}

void ForestationInstruction::initInstruction()
{
    this->boardModel->printMessage("FORESTATION:");
    this->boardModel->printMessage("");
    this->boardModel->printMessage(QString("Choose a region without a forest and decimate %1 tribes to create a forest")
                                  .arg(this->tribesCost));
    this->boardModel->printMessage("and remove the desert, if there is one.");
    this->boardModel->printMessage("Remember: At least 1 tribe has to remain anywhere in the Empire, when decimating Tribes!");
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("When you are done, press Done.");
    this->boardModel->printMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->setDoneButton(true);
    return;
}

Instruction *ForestationInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(!regionModel->hasForest() && regionModel->getTribes() >= 4 && this->boardModel->getTribeCount() > 4)
        {
            regionModel->setTribes(regionModel->getTribes() - 4);
            regionModel->setForest(true);
        }
    }

    return this;
}

Instruction *ForestationInstruction::triggerDone()
{
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();

    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
