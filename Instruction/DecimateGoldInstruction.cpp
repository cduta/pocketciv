#include "DecimateGoldInstruction.hpp"

#include "Instruction/Event/AnarchyEventInstruction.hpp"
#include "Instruction/AdvanceCityAVInstruction.hpp"
#include "Instruction/ReduceCityAVInstruction.hpp"
#include "Instruction/AssignCapitolInstruction.hpp"

DecimateGoldInstruction::DecimateGoldInstruction(BoardModel *boardModel)
    : boardModel(boardModel), anarchy(false)
{}

void DecimateGoldInstruction::initInstruction()
{
    this->boardModel->sendMessage("DECIMATE GOLD");

    int decimateGold = this->boardModel->getGold();

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::COINAGE))
    {
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::BLACK_MARKET))
        {
            decimateGold = 1;
            this->boardModel->sendMessage("Advance (COINAGE):");
            this->boardModel->sendMessage("Because of BLACK MARKET, decimate 1 Gold instead of no Gold.");
            this->boardModel->sendMessage(" ");
            this->boardModel->sendMessage("Advance (BLACK MARKET):");
            this->boardModel->sendMessage("If you cannot decimate any gold, an ANARCHY Event occurs.");

            if(this->boardModel->getGold() == 0)
            {
                this->boardModel->sendMessage(" ");
                this->boardModel->sendMessage("No Gold decimated! An ANARCHY Event occurs.");
                this->boardModel->sendMessage("Press Done to Continue...");
                this->boardModel->sendMessage(" ");
                this->anarchy = true;
                return;
            }
        }
        else
        {
            this->boardModel->sendMessage("Advance (COINAGE):");
            this->boardModel->sendMessage("Do not decimate any gold.");
            decimateGold = 0;
            return;
        }
    }
    else
    {
        this->boardModel->sendMessage("Decimate all Gold.");
        decimateGold = this->boardModel->getGold();

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::BLACK_MARKET))
        {
            this->boardModel->sendMessage("Advance (BLACK MARKET):");
            this->boardModel->sendMessage("If you cannot decimate any gold, an ANARCHY Event occurs.");
            this->boardModel->sendMessage(" ");

            if(this->boardModel->getGold() == 0)
            {
                this->boardModel->sendMessage(" ");
                this->boardModel->sendMessage("No Gold decimated! An ANARCHY Event occurs.");
                this->boardModel->sendMessage("Press Done to Continue...");
                this->boardModel->sendMessage(" ");
                this->anarchy = true;
                return;
            }
        }
    }

    this->boardModel->removeGold(decimateGold);
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage(QString("%1 gold decimated.").arg(decimateGold));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to Continue");
    this->boardModel->sendMessage(" ");
    return;
}

Instruction *DecimateGoldInstruction::triggerDone()
{
    Instruction *next;

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CENTRALIZED_GOVERNMENT) &&
       !this->boardModel->hasCapitolAssigned())
    {
        next = new AssignCapitolInstruction(this->boardModel);
    }
    else if(this->boardModel->hasAdvanceAquired(AdvanceModel::CIVIL_SERVICE) ||
            this->boardModel->hasAdvanceAquired(AdvanceModel::MASONRY) ||
            this->boardModel->hasAdvanceAquired(AdvanceModel::SLAVE_LABOR))
    {
        this->boardModel->sendMessage("To Advance City AV, you need at least one of the following advances:");
        this->boardModel->sendMessage("CIVIL SERVICE, MASONRY or SLAVE LABOR.");
        this->boardModel->sendMessage(" ");
        next = new AdvanceCityAVInstruction(this->boardModel);
    }
    else
    {
        next = new ReduceCityAVInstruction(this->boardModel);
    }

    if(this->anarchy)
    {
        next = new AnarchyEventInstruction(this->boardModel, next, NULL);
    }

    next->initInstruction();
    return next;
}

