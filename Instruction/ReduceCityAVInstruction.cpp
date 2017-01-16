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
            this->boardModel->gainGold(1);
        }

        this->boardModel->printMessage("Advance (BANKING):");
        this->boardModel->printMessage("If you have more than 3 Gold, add 1 Gold.");
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::PATRONAGE))
    {
        this->boardModel->printMessage("Advance (PATRONAGE):");
        this->boardModel->printMessage("Gain one Gold for every any city in your Empire over 4.");

        int cityCount = this->boardModel->getCityCount();
        int patronage = cityCount - 4;

        if(patronage > 0)
        {
            this->boardModel->gainGold(patronage);
            this->boardModel->printMessage(QString("You have %1 cities, therefore you gain %2 gold.").arg(cityCount).arg(patronage));
        }
        else
        {
            this->boardModel->printMessage(QString("You have %1 cities, therefore you gain no gold.").arg(cityCount));
        }

        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("REDUCE CITY AV:");
    this->boardModel->printMessage("Any city in a region without a farm to support it has its City AV reduced by 1.");
    this->boardModel->printMessage("Any city with 0 AV will be decimated.");
    this->boardModel->printMessage(" ");
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::FISHING))
    {
        this->boardModel->printMessage("Advance (FISHING):");
        this->boardModel->printMessage("Cities in Regions bordering on SEA don't need Farms to support them.");
        this->boardModel->printMessage(" ");
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CARTAGE))
    {
        this->boardModel->printMessage("Advance (CARTAGE):");
        this->boardModel->printMessage("A farm can support a single City anywhere in the Empire.");
        this->boardModel->printMessage(" ");
        this->unsupportedCities = this->boardModel->checkCartageCitySupport();

        if(this->unsupportedCities > 0)
        {
            this->boardModel->printMessage("The Empire has %1 cities not supported by farms.");
            this->boardModel->printMessage("Choose which one have their City AV reduced by 1.");
            this->boardModel->printMessage("After that, press Done to Continue...");
            this->boardModel->printMessage(" ");
        }
    }
    else
    {
        this->boardModel->checkCitySupport();
        this->boardModel->decimateZeroAVCities();
        this->boardModel->printMessage("Press Done to Continue...");
        this->boardModel->printMessage(" ");
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
        this->boardModel->printMessage("There are still %1 unsupported cities left.");
        this->boardModel->printMessage("Choose which one have their City AV reduced by 1.");
        this->boardModel->printMessage("After that, press Done to Continue...");
        this->boardModel->printMessage(" ");
    }
    else
    {
        this->boardModel->unselectAllRegions();
        this->boardModel->decimateZeroAVCities();

        Instruction *next;
        if(this->boardModel->getTribeCount() == 0)
        {
            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("The Empire has no more tribes and cities left.");
            next = new EndGameInstruction(this->boardModel, true);
        }
        else
        {
            this->boardModel->printMessage("The NEXT TURN begins...");
            this->boardModel->printMessage(" ");

            this->boardModel->unsetAdvancesAquired();

            next = new PopulationGrowthInstruction(this->boardModel);
        }

        next->initInstruction();
        return next;
    }

    return this;
}
