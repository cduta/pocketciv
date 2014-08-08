#include "EpidemicEvent.hpp"

EpidemicEvent::EpidemicEvent(BoardModel *boardModel)
    : boardModel(boardModel)
{}

QString EpidemicEvent::getEventName() const
{
    return "EPIDEMIC";
}

Instruction *EpidemicEvent::happen(Instruction *current) const
{
    return current;
}
