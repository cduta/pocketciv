#include "EarthquakeEvent.hpp"

#include "Instruction/Event/EarthquakeEventInstruction.hpp"

EarthquakeEvent::EarthquakeEvent(int redCircles, int greenSquares, int blueHexagons, BoardModel *boardModel)
    : Event(redCircles, greenSquares, blueHexagons), boardModel(boardModel)
{}

QString EarthquakeEvent::getEventName() const
{
    return "EARTHQUAKE";
}

Instruction *EarthquakeEvent::happen(Instruction *next) const
{
    return new EarthquakeEventInstruction(this->boardModel, next, this);
}
