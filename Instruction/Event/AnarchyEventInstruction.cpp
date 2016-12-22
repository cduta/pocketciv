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
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::LAW))
    {
        this->boardModel->printMessage("Advance (LAW):");
        this->boardModel->printMessage("In every region with a city, decimate as many tribes");
        this->boardModel->printMessage("as the City AV of that city.");
        this->boardModel->printMessage(" ");

        foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
        {
            if(regionModel->hasCity())
            {
                int tribes = regionModel->getTribes();
                int cityAV = regionModel->getCityAV();

                regionModel->decimateTribes(cityAV);

                this->boardModel->printMessage(QString("In region %1 there have been %2 tribes decimated.")
                                              .arg(regionModel->getRegion())
                                              .arg(tribes - regionModel->getTribes()));
            }
        }
    }
    else
    {
        this->boardModel->printMessage("In every region with a city, reduce 1 City AV and decimate 3 tribes.");
        this->boardModel->printMessage("This is repeated in every region, until the tribes are");
        this->boardModel->printMessage("lower than the City AV or the City AV is 1 in each region.");
        this->boardModel->printMessage(" ");

        foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
        {
            if(regionModel->hasCity() && regionModel->getCityAV() > 0)
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


                int decimatedTribes = tribes-regionModel->getTribes();

                QString tribeString = "tribe";

                if(decimatedTribes > 1)
                {
                    tribeString = "tribes";
                }

                this->boardModel->printMessage(QString("In region %1 there have been %2 City AV reduced and %3 %4 decimated.")
                                              .arg(regionModel->getRegion())
                                              .arg(cityAV-regionModel->getCityAV())
                                              .arg(decimatedTribes)
                                              .arg(tribeString));
            }
        }
    }

    return this->endEvent();
}

