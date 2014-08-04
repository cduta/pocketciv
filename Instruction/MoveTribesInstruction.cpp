#include "MoveTribesInstruction.hpp"

MoveTribesInstruction::MoveTribesInstruction(BoardModel *boardModel)
    : Instruction(boardModel)
{
    this->boardModel->sendMessage("Done creating the world!");
    this->boardModel->clearMessages();
    this->boardModel->sendMessage("Discarding 3 event cards...");
    this->boardModel->drawCard();
    this->boardModel->drawCard();
    this->boardModel->drawCard();
    this->boardModel->sendMessage("The Game begins...");
    this->boardModel->sendMessage("Population Growth:");
    this->boardModel->sendMessage("Added 1 tribe to any region with at least 1 Tribe on it.");
    this->boardModel->populationGrowth();
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Population Movement:");
    this->boardModel->sendMessage("Move any tribe once.");
    this->boardModel->sendMessage("When you are done, press Done...");
}

Instruction *MoveTribesInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

        if(regionModel->getTribes() > 0)
        {
            // MoveTribesToInstruction (regionModel, ...) here, change done to cancel.
            // After move to is chosen, dialog with "how many" with ok and cancel.
        }
    }

    return this;
}

Instruction *MoveTribesInstruction::triggerDone()
{
    // Done, merge all moved tribes with tribes in all regions.
    // Next: Draw Event Card (Return instructions accordingly).

    return this;
}
