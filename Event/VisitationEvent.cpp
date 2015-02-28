#include "VisitationEvent.hpp"

#include "Instruction/Event/VisitationEventInstruction.hpp"
#include "Common.hpp"

VisitationEvent::VisitationEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel::Empire empire, BoardModel *boardModel)
    : Event(redCircles, greenSquares, blueHexagons), empire(empire), boardModel(boardModel)
{}

QString VisitationEvent::getEventName() const
{
    return QString("VISITATION: %1").arg(Common::getEmpireName(this->empire));
}

Instruction *VisitationEvent::happen(Instruction *next) const
{
    return new VisitationEventInstruction(this->empire, this->boardModel, next, this);
}
