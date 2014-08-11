#include "UpkeepInstruction.hpp"

#include "Instruction/PopulationGrowthInstruction.hpp"

UpkeepInstruction::UpkeepInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel), done(false)
{}

void UpkeepInstruction::initInstruction()
{
    this->boardModel->clearMessages();
    this->boardModel->sendMessage("Upkeep:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Decimate unsupported tribes.");
    this->boardModel->sendMessage("A region can support as much tribes as there are Mountains,");
    this->boardModel->sendMessage("Volcanoes, Farms, Forests and and City AV in that region added up.");
    this->boardModel->decimateUnsupportedTribes();
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Decimate all Gold.");
    this->boardModel->setGold(0);
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Advance City AV.");
    this->boardModel->sendMessage("To increase a city's AV, choose a city and decimate");
    this->boardModel->sendMessage("as much tribes from anywhere in the EMPIRE until the amount");
    this->boardModel->sendMessage("of decimated tribes is equal the chosen City AV plus 1.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press done.");
    this->boardModel->sendMessage(" ");
}

Instruction *UpkeepInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);
    RegionModel *activeRegion = this->boardModel->refActiveRegion();

    if(button == Qt::LeftButton)
    {
        if(activeRegion == NULL)
        {
            this->boardModel->setActiveRegion(regionModel->getRegion());
            this->boardModel->sendDoneText("Cancel");
        }
        else if(activeRegion != NULL && regionModel->getTribes() - regionModel->getSelectedTribes() > 0)
        {
            int selectedTribes = this->boardModel->getAllSelectedTribes();
            int toBePaid = activeRegion->getCityAV()+1;

            if(toBePaid < selectedTribes)
            {
                regionModel->setSelectedTribes(regionModel->getSelectedTribes() + 1);
                selectedTribes++;
            }

            if(toBePaid == selectedTribes)
            {
                this->boardModel->sendDoneText("Advance");
            }
            else
            {
                this->boardModel->sendMessage(QString("%1/%2 tribes paid to advance the City AV in region %3.")
                                              .arg(selectedTribes)
                                              .arg(toBePaid)
                                              .arg(activeRegion->getRegion()));
            }
        }
    }else if(button == Qt::RightButton)
    {
        if(activeRegion != NULL)
        {
            int selectedTribes = this->boardModel->getAllSelectedTribes();
            int toBePaid = activeRegion->getCityAV()+1;

            if(selectedTribes > 0)
            {
                regionModel->setSelectedTribes(regionModel->getSelectedTribes() - 1);
                selectedTribes--;

                this->boardModel->sendMessage(QString("%1/%2 tribes paid to advance the City AV in region %3.")
                                              .arg(selectedTribes)
                                              .arg(toBePaid)
                                              .arg(activeRegion->getRegion()));
            }
        }
    }

    return this;
}

Instruction *UpkeepInstruction::triggerDone()
{
    this->boardModel->sendDoneText("Done");

    if(this->boardModel->refActiveRegion() != NULL)
    {
        RegionModel *activeRegion = this->boardModel->refActiveRegion();
        int selectedTribes = this->boardModel->getAllSelectedTribes();
        int toBePaid = activeRegion->getCityAV()+1;

        if(selectedTribes == toBePaid)
        {
            this->boardModel->sendMessage(QString("The City AV in region %1 has been advanced to %2.")
                                          .arg(activeRegion->getRegion())
                                          .arg(activeRegion->getCityAV()));
            this->boardModel->decimateAllSelectedTribes();
        }
        else
        {
            this->boardModel->unselectAllSelectedTribes();
        }

        this->boardModel->unsetActiveRegion();
        return this;
    }

    if(!this->done)
    {
        this->boardModel->sendMessage("Reduce City AV:");
        this->boardModel->sendMessage("Any city in a region without a farm has its AV reduced by 1.");
        this->boardModel->sendMessage("Any city with 0 AV will be decimated.");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Press done to continue.");
        this->boardModel->sendMessage(" ");
        this->boardModel->checkCitySupport();
        this->boardModel->decimateZeroAVCities();
        this->done = true;
    }

    if(this->done)
    {
        Instruction *next = new PopulationGrowthInstruction(this->boardModel);
        next->initInstruction();
        return next;
    }

    return this;
}