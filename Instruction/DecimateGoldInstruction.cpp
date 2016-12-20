#include "DecimateGoldInstruction.hpp"

#include "Instruction/AdvanceCityAVInstruction.hpp"
#include "Instruction/ReduceCityAVInstruction.hpp"
#include "Instruction/AssignCapitolInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

DecimateGoldInstruction::DecimateGoldInstruction(BoardModel *boardModel)
    : boardModel(boardModel), step(0)
{}

void DecimateGoldInstruction::initInstruction()
{
    this->boardModel->printMessage("DECIMATE GOLD");

    int decimateGold = this->boardModel->getGold();

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::COINAGE))
    {
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::BLACK_MARKET))
        {
            decimateGold = 1;
            this->boardModel->printMessage("Advance (COINAGE):");
            this->boardModel->printMessage("Because of BLACK MARKET, decimate 1 Gold instead of no Gold.");
            this->boardModel->printMessage(" ");
            this->boardModel->printMessage("Advance (BLACK MARKET):");
            this->boardModel->printMessage("If you cannot decimate any gold, an ANARCHY Event occurs.");
        }
        else
        {
            this->boardModel->printMessage("Advance (COINAGE):");
            this->boardModel->printMessage("Do not decimate any gold.");
            decimateGold = 0;
            return;
        }
    }
    else
    {
        this->boardModel->printMessage("Decimate all Gold.");
        decimateGold = this->boardModel->getGold();
    }

    this->boardModel->removeGold(decimateGold);
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage(QString("%1 gold decimated.").arg(decimateGold));
    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to Continue");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *DecimateGoldInstruction::triggerDone()
{
    Instruction *next;

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CULTURE_OF_THIEVES) && this->step == 0)
    {
        this->step = 1;

        this->boardModel->printMessage("Advance (CULTURE OF THIEVES):");
        this->boardModel->setActiveRegion(this->boardModel->drawCard()->getShapeNumbers().value(Event::RED_CIRCLE, 0), true);
        this->boardModel->printMessage(QString("The active region is %1.").arg(this->boardModel->refActiveRegion()->getRegion()));
        this->boardModel->printMessage(" ");
        this->boardModel->printMessage("If the active region has at least 1 Tribe, decimate 1 tribe and draw another Event card.\n"
                                      "Gain gold equal to the amount of Gold Nuggets on the Event Card.\n\n");

        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CULTURE_OF_THIEVES) && this->step == 1)
    {
        this->step = 2;

        RegionModel *activeRegion = this->boardModel->refActiveRegion();

        if(activeRegion->getTribes() > 0)
        {
            this->thieveryCard = this->boardModel->drawCard();
            POCKET_CIV_END_OF_ERA_CHECK
        }
        else
        {
            this->step = -1;
            this->boardModel->printMessage("The active region had no tribes to be decimated and therefore nothing happens.\n\n");
        }
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CULTURE_OF_THIEVES) && this->step == 2)
    {
        this->step = -1;
        this->boardModel->refActiveRegion()->decimateTribes(1);
        int goldGained = this->thieveryCard->getGoldNuggets();
        this->boardModel->gainGold(goldGained);
        this->boardModel->printMessage(QString("After decimating a tribe in the active region, you gain %1 gold.\n\n")
                                      .arg(goldGained));
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CENTRALIZED_GOVERNMENT) &&
       !this->boardModel->hasCapitolAssigned())
    {
        next = new AssignCapitolInstruction(this->boardModel);
    }
    else if(this->boardModel->hasAdvanceAquired(AdvanceModel::CIVIL_SERVICE) ||
            this->boardModel->hasAdvanceAquired(AdvanceModel::MASONRY) ||
            this->boardModel->hasAdvanceAquired(AdvanceModel::SLAVE_LABOR))
    {
        this->boardModel->printMessage("To Advance City AV, you need at least one of the following advances:");
        this->boardModel->printMessage("CIVIL SERVICE, MASONRY or SLAVE LABOR.");
        this->boardModel->printMessage(" ");
        next = new AdvanceCityAVInstruction(this->boardModel);
    }
    else
    {
        next = new ReduceCityAVInstruction(this->boardModel);
    }

    next->initInstruction();
    return next;
}

