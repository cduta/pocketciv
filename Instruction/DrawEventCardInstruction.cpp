#include "DrawEventCardInstruction.hpp"

DrawEventCardInstruction::DrawEventCardInstruction(BoardModel *boardModel, Instruction *nextInstruction, const EventCard *eventCard)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), originalEventCard(eventCard), currentEvent(NULL)
{}

void DrawEventCardInstruction::initInstruction()
{
    this->boardModel->clearMessages();
    this->boardModel->sendMessage("Draw Event Card...");
    this->boardModel->sendMessage(" ");

    const Event *event = this->originalEventCard->getEvent(this->boardModel->getEra());

    if(event != NULL)
    {
        this->boardModel->sendMessage(QString("Event: %1").arg(event->getEventName()));
        this->nextInstruction = event->happen(this->nextInstruction);
    }
    else
    {
        this->boardModel->sendMessage("Nothing occured...");
    }

    this->boardModel->sendMessage("Press Done to continue.");
}
