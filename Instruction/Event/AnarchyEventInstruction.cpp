#include "AnarchyEventInstruction.hpp"

AnarchyEventInstruction::AnarchyEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event)
{
    this->nextInstruction->setKeepInstruction(true);
}

void AnarchyEventInstruction::initInstruction()
{
    this->boardModel->printMessage("ANARCHY:");
    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *AnarchyEventInstruction::triggerDone()
{
    this->boardModel->printMessage("In every region with a city, reduce 1 City AV and decimate 3 tribes.");
    this->boardModel->printMessage("This is repeated in every region, until the tribes are");
    this->boardModel->printMessage("lower than the City AV or the City AV is 1 in each region.");
    this->boardModel->printMessage(" ");

    foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
    {
        if(regionModel->hasCity())
        {
            int cityAV = regionModel->getCityAV();
            int tribes = regionModel->getTribes();

            do
            {
                regionModel->setCityAV(regionModel->getCityAV()-1);
                regionModel->setTribes(regionModel->getTribes()-3);
            }
            while(regionModel->getCityAV() > 1 &&
                  regionModel->getCityAV() <= regionModel->getTribes());


            this->boardModel->printMessage(QString("Region %1: %2 city AV and %3 tribes have been decimated.")
                                          .arg(regionModel->getRegion())
                                          .arg(cityAV-regionModel->getCityAV())
                                          .arg(tribes-regionModel->getTribes()));
        }
    }

    return this->endEvent();
}

