#include "EventInstruction.hpp"

EventInstruction::EventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event, QObject *parent)
    : Instruction(parent), boardModel(boardModel), nextInstruction(nextInstruction), event(event)
{}

void EventInstruction::drawActiveRegion()
{
    this->boardModel->setActiveRegion(this->boardModel->drawCard()->getShapeNumbers().value(Event::RED_CIRCLE, 0), true);
    this->boardModel->sendMessage(QString("The active region is %1.").arg(this->boardModel->refActiveRegion()->getRegion()));
    this->boardModel->sendMessage(" ");
    return;
}

Instruction *EventInstruction::endEvent()
{
    this->boardModel->sendMessage("The event has ended...");
    this->boardModel->sendMessage(" ");
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
