#include "AdvanceCityAVInstruction.hpp"

#include "Instruction/ReduceCityAVInstruction.hpp"

AdvanceCityAVInstruction::AdvanceCityAVInstruction(BoardModel *boardModel)
    : boardModel(boardModel)
{}

void AdvanceCityAVInstruction::initInstruction()
{
    this->boardModel->sendMessage("Advance City AV.");
    this->boardModel->sendMessage("To increase a city's AV, pay with tribes from anywhere in the Empire.");
    this->boardModel->sendMessage(" ");
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::BASIC_TOOLS))
    {
        this->boardModel->sendMessage("Advance (BASIC TOOLS):");
        this->boardModel->sendMessage("The cost of tribes to advance a City AV is reduced by 1.");
        this->boardModel->sendMessage(" ");
    }

    if(this->boardModel->getAdvancesAquired().contains(AdvanceModel::ARCHITECTURE))
    {
        this->maximumCityAV = 4;
        this->boardModel->sendMessage("Advance (AGRICULTURE):");
        this->boardModel->sendMessage("The maximum City AV is 4.");
        this->boardModel->sendMessage(" ");
    }
    else
    {
        this->maximumCityAV = 2;
        this->boardModel->sendMessage("The maximum City AV is 2.");
        this->boardModel->sendMessage(" ");
    }

    this->boardModel->sendMessage("Press Done to Continue...");
}

Instruction *AdvanceCityAVInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
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
                    this->boardModel->sendMessage("You can use tribes from anywhere in the Empire for this.");
                    this->boardModel->sendMessage(" ");

                    this->boardModel->setActiveRegion(regionModel->getRegion(), true);
                    this->boardModel->sendDoneText("Cancel");
                }
                else
                {
                    this->boardModel->sendMessage(QString("You can't advance the City AV in Region %1 any further than %2.")
                                                  .arg(regionModel->getRegion())
                                                  .arg(this->maximumCityAV));
                    this->boardModel->sendMessage(" ");
                }
            }
            else
            {
                this->boardModel->sendMessage(QString("There is no City in Region %1.")
                                              .arg(regionModel->getRegion()));
                this->boardModel->sendMessage(" ");
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

Instruction *AdvanceCityAVInstruction::triggerDone()
{
    if(this->boardModel->refActiveRegion() != NULL)
    {
        this->boardModel->sendMessage(" ");
        RegionModel *activeRegion = this->boardModel->refActiveRegion();
        int selectedTribes = this->boardModel->getAllSelectedTribes();

        if(selectedTribes == this->toBePaid)
        {
            this->boardModel->sendMessage(QString("The City AV in region %1 has been advanced to %2.")
                                          .arg(activeRegion->getRegion())
                                          .arg(activeRegion->getCityAV()));
            this->boardModel->decimateAllSelectedTribes();
        }
        else
        {
            this->boardModel->sendMessage("Advancing City AV cancelled.");
            this->boardModel->unselectAllSelectedTribes();
            this->boardModel->setDoneButton("Done");
        }

        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("Choose another City to advance or press Done to Continue...");
        this->boardModel->sendMessage(" ");
        this->boardModel->unsetActiveRegion();
        return this;
    }

    Instruction *next = new ReduceCityAVInstruction(this->boardModel);
    next->initInstruction();
    return next;
}
