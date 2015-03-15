#include "ReduceCityAVInstruction.hpp"

#include "Instruction/EndGameInstruction.hpp"
#include "Instruction/PopulationGrowthInstruction.hpp"

ReduceCityAVInstruction::ReduceCityAVInstruction(BoardModel *boardModel)
    : boardModel(boardModel), unsupportedCities(0)
{}

void ReduceCityAVInstruction::initInstruction()
{
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::BANKING))
    {
        if(this->boardModel->getGold() > 3)
        {
            this->boardModel->addGold(1);
        }

        this->boardModel->sendMessage("Advance (BANKING):");
        this->boardModel->sendMessage("If you have more than 3 Gold, add 1 Gold.");
    }

    this->boardModel->sendMessage("REDUCE CITY AV:");
    this->boardModel->sendMessage("Any city in a region without a farm to support it has its City AV reduced by 1.");
    this->boardModel->sendMessage("Any city with 0 AV will be decimated.");
    this->boardModel->sendMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CARTAGE))
    {
        this->boardModel->sendMessage("Advance (CARTAGE):");
        this->boardModel->sendMessage("A farm can support a single City anywhere in the Empire.");
        this->boardModel->sendMessage(" ");
        this->unsupportedCities = this->boardModel->checkCartageCitySupport();

        if(this->unsupportedCities > 0)
        {
            this->boardModel->sendMessage("The Empire has %1 cities not supported by farms.");
            this->boardModel->sendMessage("Choose which one have their City AV reduced by 1.");
            this->boardModel->sendMessage("After that, press Done to Continue...");
            this->boardModel->sendMessage(" ");
        }
    }
    else
    {
        this->boardModel->checkCitySupport();
        this->boardModel->decimateZeroAVCities();
        this->boardModel->sendMessage("Press Done to Continue...");
        this->boardModel->sendMessage(" ");
    }
}

Instruction *ReduceCityAVInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton && !regionModel->isSelected() && this->unsupportedCities > 0)
    {
        this->boardModel->setSelectRegion(regionModel->getRegion(), true);
        regionModel->decreaseCityAV(1);
        this->unsupportedCities--;
    }
    else if(button == Qt::RightButton && regionModel->isSelected())
    {
        this->boardModel->setSelectRegion(regionModel->getRegion(), false);
        regionModel->increaseCityAV(1);
        this->unsupportedCities++;
    }

    return this;
}

Instruction *ReduceCityAVInstruction::triggerDone()
{
    if(this->unsupportedCities > 0)
    {
        this->boardModel->sendMessage("There are still %1 unsupported cities left.");
        this->boardModel->sendMessage("Choose which one have their City AV reduced by 1.");
        this->boardModel->sendMessage("After that, press Done to Continue...");
        this->boardModel->sendMessage(" ");
    }
    else
    {
        this->boardModel->unselectAllRegions();

        Instruction *next;
        if(this->boardModel->getTribeCount() == 0 && !this->boardModel->hasCity())
        {
            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage("The Empire had no more tribes and cities left.");
            next = new EndGameInstruction(this->boardModel);
        }
        else
        {
            this->boardModel->sendMessage("The NEXT TURN begins...");
            this->boardModel->sendMessage(" ");

            this->boardModel->unsetAdvancesAquired();

            next = new PopulationGrowthInstruction(this->boardModel);
        }

        next->initInstruction();
        return next;
    }

    return this;
}
