#include "EventInstruction.hpp"

EventInstruction::EventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event, QObject *parent)
    : Instruction(parent), boardModel(boardModel), nextInstruction(nextInstruction), event(event)
{}

void EventInstruction::drawActiveRegion(bool isBad)
{
    this->boardModel->setActiveRegion(this->boardModel->drawCard()->getShapeNumbers().value(Event::RED_CIRCLE, 0), isBad);
    this->boardModel->printMessage(QString("The active region is %1.").arg(this->boardModel->refActiveRegion()->getRegion()));
    this->boardModel->printMessage(" ");
    return;
}

Instruction *EventInstruction::endEvent()
{
    this->boardModel->printMessage("The event has ended...");
    this->boardModel->printMessage(" ");
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
