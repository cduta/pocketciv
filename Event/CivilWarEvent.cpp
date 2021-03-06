#include "CivilWarEvent.hpp"

#include "Instruction/Event/CivilWarEventInstruction.hpp"

CivilWarEvent::CivilWarEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel)
    : Event(redCircles, greenSquares, blueHexagons), boardModel(boardModel)
{}

QString CivilWarEvent::getEventName() const
{
    return "CIVIL WAR";
}

Instruction *CivilWarEvent::happen(Instruction *next) const
{
    return new CivilWarEventInstruction(this->boardModel, next, this);
}
