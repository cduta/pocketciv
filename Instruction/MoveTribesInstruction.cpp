#include "MoveTribesInstruction.hpp"

#include "HowManyDialog.hpp"
#include "Instruction/DrawEventCardInstruction.hpp"
#include "Instruction/MainPhaseInstruction.hpp"
#include "Instruction/DialogInstruction.hpp"

MoveTribesInstruction::MoveTribesInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel)
{}

void MoveTribesInstruction::initInstruction()
{
    this->boardModel->sendMessage("Done creating the world!");
    this->boardModel->clearMessages();
    this->boardModel->sendMessage("Discarding 3 event cards...");
    this->boardModel->drawCard();
    this->boardModel->drawCard();
    this->boardModel->drawCard();
    this->boardModel->sendMessage("The Game begins...");
    this->boardModel->sendMessage("Population Growth:");
    this->boardModel->sendMessage("Added 1 tribe to any region with at least 1 Tribe on it.");
    this->boardModel->populationGrowth();
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Population Movement:");
    this->boardModel->sendMessage("Move tribes from one to another region. Every tribe can be moved once.");
    this->boardModel->sendMessage("When you are done, press Done...");
}

Instruction *MoveTribesInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    if(button == Qt::LeftButton)
    {
        RegionModel *regionModel = this->boardModel->refRegionModel(x,y);
        if(regionModel == NULL)
        {
            return this;
        }

        QList<RegionModel *> selectedRegions = this->boardModel->getSelectedRegions().values();

        if(selectedRegions.isEmpty())
        {

            if(regionModel->getTribes() > 0)
            {
                this->boardModel->setSelectRegion(regionModel->getRegion(), true);
            }
        }
        else if(selectedRegions.size() == 1)
        {
            if(selectedRegions.contains(regionModel))
            {
                this->boardModel->setSelectRegion(regionModel->getRegion(), false);
            }
            else
            {
                RegionModel *fromRegion = selectedRegions.first();
                RegionModel *toRegion = this->boardModel->refRegionModel(x,y);

                if(this->boardModel->canMoveTribes(fromRegion->getRegion(), toRegion->getRegion()))
                {
                    this->boardModel->setSelectRegion(toRegion->getRegion(), true);
                    return new DialogInstruction(
                                this->boardModel,
                                this,
                                new MoveTribeDialog(
                                    this->boardModel,
                                    QString("of %1 Tribes from region %2 to region %3.")
                                    .arg(QString::number(fromRegion->getTribes()))
                                    .arg(fromRegion->getRegion())
                                    .arg(toRegion->getRegion()),
                                    fromRegion->getTribes(),
                                    fromRegion,
                                    toRegion)
                                );
                }
            }
        }
    }

    return this;
}

Instruction *MoveTribesInstruction::triggerDone()
{
    this->boardModel->mergeTribesAllRegions();
    this->boardModel->unselectAllRegions();

    Instruction *next = new DrawEventCardInstruction(this->boardModel, new MainPhaseInstruction(this->boardModel), this->boardModel->drawCard());
    next->initInstruction();
    return next;
}
