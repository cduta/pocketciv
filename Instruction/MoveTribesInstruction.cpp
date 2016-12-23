#include "MoveTribesInstruction.hpp"

#include "Instruction/MainPhaseInstruction.hpp"
#include "Instruction/DialogInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"

MoveTribesInstruction::MoveTribesInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel), moveTribeDialog(NULL)
{}

MoveTribesInstruction::~MoveTribesInstruction()
{
    this->moveTribeDialog->deleteLater();
}

void MoveTribesInstruction::initInstruction()
{
    this->boardModel->printMessage("POPULATION MOVEMENT:");
    this->boardModel->printMessage("Move tribes from one to another region.");
    this->boardModel->printMessage(" ");
    if(this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN) ||
       this->boardModel->hasAdvanceAquired(AdvanceModel::FISHING))
    {
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::EQUESTRIAN))
        {
            this->boardModel->printMessage("Advance (EQUESTRIAN):");
            this->boardModel->printMessage("Every tribe can be moved once into any");
            this->boardModel->printMessage("region not seperated by a SEA or FRONTIER.");
            this->boardModel->printMessage(" ");
        }

        if(this->boardModel->hasAdvanceAquired(AdvanceModel::FISHING))
        {
            this->boardModel->printMessage("Advance (FISHING):");
            this->boardModel->printMessage("Tribes on a region bordering a SEA can be moved");
            this->boardModel->printMessage("to any other region also bordering the same SEA.");
            this->boardModel->printMessage("But doing so, decimates 1 tribe from the moved group.");
            this->boardModel->printMessage(" ");

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::MAGNETICS))
            {
                this->boardModel->printMessage("Advance (MAGNETICS):");
                this->boardModel->printMessage("Moving tribes overseas does not decimate 1 tribe.");
                this->boardModel->printMessage(" ");
            }
        }
    }
    else
    {
        this->boardModel->printMessage("Every tribe can be moved once into an adjacent region.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("When you are done, press Done...");
    this->boardModel->printMessage(" ");
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

                BoardModel::MoveTribesType movementType = this->boardModel->getMoveTribesType(fromRegion->getRegion(), toRegion->getRegion());

                if(movementType > 0)
                {
                    QString by = "by land";
                    if(movementType == BoardModel::OVERSEAS)
                    {
                        by = "overseas,\nlosing one tribe on their way [Advance (FISHING)]";
                    }

                    this->boardModel->setSelectRegion(toRegion->getRegion(), true);
                    this->moveTribeDialog = new MoveTribeDialog(
                                this->boardModel,
                                QString("of %1 Tribes from region %2 to region %3 %4.")
                                .arg(QString::number(fromRegion->getTribes()))
                                .arg(fromRegion->getRegion())
                                .arg(toRegion->getRegion())
                                .arg(by),
                                fromRegion->getTribes(),
                                fromRegion,
                                toRegion,
                                movementType);
                    return new DialogInstruction(
                                this->boardModel,
                                this,
                                this->moveTribeDialog);

                }
            }
        }
    }

    return this;
}

Instruction *MoveTribesInstruction::triggerDone()
{
    this->boardModel->mergeAllMovedTribes();
    this->boardModel->unselectAllRegions();
    this->boardModel->printMessage(" ");

    const Event *event = this->boardModel->drawOriginalCard()->getEvent(this->boardModel->getEra());
    Instruction *main = new MainPhaseInstruction(this->boardModel);

    if(event == NULL)
    {
        this->boardModel->printMessage("Nothing occured...");
        this->boardModel->printMessage(" ");
        main->initInstruction();
        return main;
    }

    Instruction *eventInstruction = event->happen(main);
    eventInstruction->initInstruction();
    Instruction *next;

    if(this->boardModel->isEndOfEra())
    {
        next = new EndOfEraInstruction(this->boardModel, eventInstruction);
        next->initInstruction();
    }
    else
    {
        next = eventInstruction;
    }

    return next;
}
