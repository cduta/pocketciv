#include "BuildWonderInstruction.hpp"

BuildWonderInstruction::BuildWonderInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : boardModel(boardModel),
      nextInstruction(nextInstruction),
      wonderDialog(NULL)
{
    this->nextInstruction->setKeepInstruction(true);
}

void BuildWonderInstruction::initInstruction()
{
    this->boardModel->printMessage("BUILD WONDER:");

    this->boardModel->printMessage("Choose a region in which to build a wonder");
    this->boardModel->printMessage("using the ressources of that region.");
    this->boardModel->printMessage("Remember: At least 1 tribe has to remain anywhere in the Empire, when decimating Tribes!");
    this->boardModel->printMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->setDoneButton(true);
}

Instruction *BuildWonderInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel)
    {
        if(button == Qt::LeftButton && !this->boardModel->isBuildingWonders())
        {
            this->boardModel->setBuildingWonders(true);
            this->boardModel->setDoneButton(false);
            this->boardModel->setActiveRegion(regionModel->getRegion(), false);
            this->wonderDialog = new WonderDialog(this->boardModel, WonderDescription::BUILD);
            connect(this->wonderDialog, SIGNAL(finished(int)), this, SLOT(doneBuildingWonders()));
            this->wonderDialog->show();
        }
    }

    return this;
}

Instruction *BuildWonderInstruction::triggerDone()
{
    if(this->boardModel->isBuildingWonders())
    {
        return this;
    }

    this->boardModel->unselectAllRegions();
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();
    this->nextInstruction->setKeepInstruction(false);
    return this->nextInstruction;
}

void BuildWonderInstruction::doneBuildingWonders()
{
    disconnect(this->wonderDialog, SIGNAL(finished(int)), this, SLOT(doneBuildingWonders()));

    this->boardModel->setBuildingWonders(false);
    this->boardModel->setDoneButton(true);
    this->boardModel->unsetActiveRegion();
    this->boardModel->unselectAllRegions();

    this->wonderDialog->deleteLater();
    return;
}
