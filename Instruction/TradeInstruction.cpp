#include "TradeInstruction.hpp"

#include "Instruction/EndOfEraInstruction.hpp"

TradeInstruction::TradeInstruction(BoardModel *boardModel, Instruction *nextInstruction, QString what)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction), what(what), step(0), goldGain(0)
{}

void TradeInstruction::initInstruction()
{
    this->boardModel->sendMessage(QString("%1 TRADE:").arg(this->what));
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done to continue.");
    this->boardModel->sendMessage(" ");
    return;
}

Instruction *TradeInstruction::triggerDone()
{
    if(this->step == 0)
    {
        this->step++;
        this->goldGain = this->boardModel->drawCard()->getGoldNuggets();
        POCKET_CIV_END_OF_ERA_CHECK
    }

    if(this->step == 1)
    {
        this->step++;
        this->boardModel->sendMessage(QString("When trading with %1, you gained %2 gold.").arg(this->what).arg(this->goldGain));
        this->boardModel->addGold(this->goldGain);
    }

    this->boardModel->sendMessage(QString("The %1 TRADE has ended.").arg(this->what));
    this->boardModel->sendMessage(" ");

    this->boardModel->unsetActiveRegion();
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
