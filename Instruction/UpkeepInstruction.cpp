#include "UpkeepInstruction.hpp"

#include "Instruction/PopulationGrowthInstruction.hpp"
#include "Instruction/EndGameInstruction.hpp"

UpkeepInstruction::UpkeepInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel), done(false)
{

}

void UpkeepInstruction::initInstruction()
{
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("UPKEEP:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Decimate unsupported tribes.");
    this->boardModel->sendMessage("A region can support as much tribes as there are Mountains,");
    this->boardModel->sendMessage("Volcanoes, Farms, Forests and City AV in that region added up.");
    this->boardModel->decimateUnsupportedTribes();
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Decimate all Gold.");
    this->boardModel->setGold(0);
    this->boardModel->sendMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::BASIC_TOOLS))
    {
        this->boardModel->sendMessage("Advance City AV.");
        this->boardModel->sendMessage("To increase a city's AV, pay with tribes from anywhere in the Empire.");
        this->boardModel->sendMessage(" ");

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::BASIC_TOOLS))
        {
            this->boardModel->sendMessage("BASIC TOOLS:");
            this->boardModel->sendMessage("The cost of tribes to advance a City AV is reduced by 1.");
            this->boardModel->sendMessage(" ");
        }

        if(this->boardModel->getAdvancesAquired().contains(AdvanceModel::ARCHITECTURE))
        {
            this->maximumCityAV = 4;
            this->boardModel->sendMessage("AGRICULTURE:");
            this->boardModel->sendMessage("The maximum City AV is 4.");
            this->boardModel->sendMessage(" ");
        }
        else
        {
            this->maximumCityAV = 2;
            this->boardModel->sendMessage("The maximum City AV is 2.");
            this->boardModel->sendMessage(" ");
        }

        this->boardModel->sendMessage("When you are done, press done.");
        this->boardModel->sendMessage(" ");
    }
    else
    {
        this->endTurn();
        this->boardModel->sendMessage("Press done to continue with the next turn.");
        this->boardModel->sendMessage(" ");
    }
}

Instruction *UpkeepInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->done)
    {
        return this;
    }

    RegionModel *activeRegion = this->boardModel->refActiveRegion();

    if(button == Qt::LeftButton)
    {
        if(activeRegion == NULL)
        {
            if(regionModel->hasCity())
            {
                if(regionModel->getCityAV() < this->maximumCityAV)
                {
                    this->toBePaid = activeRegion->getCityAV()+1;

                    if(this->boardModel->hasAdvanceAquired(AdvanceModel::BASIC_TOOLS))
                    {
                        this->toBePaid--;
                    }

                    this->boardModel->sendMessage(QString("The tribes cost of advancing the City AV in Region %1 is %2.")
                                                  .arg(activeRegion->getRegion())
                                                  .arg(this->toBePaid));
                    this->boardModel->sendMessage(" ");

                    this->boardModel->setActiveRegion(regionModel->getRegion(), true);
                    this->boardModel->sendDoneText("Cancel");
                }
                else
                {
                    this->boardModel->sendMessage(QString("You can't advance the City AV in Region %1 any further than %2.")
                                                  .arg(regionModel->getRegion())
                                                  .arg(this->maximumCityAV));
                }
            }
        }
        else if(activeRegion != NULL && (regionModel->getTribes() - regionModel->getSelectedTribes()) > 0)
        {
            int selectedTribes = this->boardModel->getAllSelectedTribes();

            if(this->toBePaid < selectedTribes)
            {
                regionModel->setSelectedTribes(regionModel->getSelectedTribes() + 1);
                selectedTribes++;
            }

            if(this->toBePaid == selectedTribes)
            {
                this->boardModel->sendDoneText("Advance");
            }
            else
            {
                this->boardModel->sendMessage(QString("%1/%2 tribes selected.")
                                              .arg(selectedTribes)
                                              .arg(toBePaid)
                                              .arg(activeRegion->getRegion()));
            }
        }
    }
    else if(button == Qt::RightButton)
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

        if(selectedTribes == this->toBePaid)
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
        this->endTurn();
        this->boardModel->sendMessage("Press done to continue with the next turn.");
        return this;
    }

    if(this->done)
    {
        Instruction *next;
        if(this->boardModel->getTribeCount() == 0 && !this->boardModel->hasCity())
        {
            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage("The Empire had no more tribes and cities left.");
            next = new EndGameInstruction(this->boardModel);
        }
        else
        {
            next = new PopulationGrowthInstruction(this->boardModel);
        }
        next->initInstruction();
        return next;
    }

    return this;
}

void UpkeepInstruction::endTurn()
{
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::BANKING))
    {
        if(this->boardModel->getGold() > 3)
        {
            this->boardModel->addGold(1);
        }

        this->boardModel->sendMessage("BANKING:");
        this->boardModel->sendMessage("If you have more than 3 Gold, add 1 Gold.");
    }

    this->boardModel->sendMessage("REDUCE CITY AV:");
    this->boardModel->sendMessage("Any city in a region without a farm has its AV reduced by 1.");
    this->boardModel->sendMessage("Any city with 0 AV will be decimated.");
    this->boardModel->sendMessage(" ");
    this->boardModel->checkCitySupport();
    this->boardModel->decimateZeroAVCities();
    this->done = true;
}
