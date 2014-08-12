#include "MainPhaseInstruction.hpp"

#include "UpkeepInstruction.hpp"

MainPhaseInstruction::MainPhaseInstruction(BoardModel *boardModel)
    : Instruction(), boardModel(boardModel), initialized(false), buildCity(false)
{}

void MainPhaseInstruction::initInstruction()
{
    this->boardModel->sendMessage("Main Phase:");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("Press Done, to begin.");
    return;
}

Instruction *MainPhaseInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(this->buildCity)
        {
            if(!regionModel->hasCity() && regionModel->getTribes() >= 4)
            {
                regionModel->setTribes(regionModel->getTribes() - 4);
                regionModel->setCityAV(1);
                regionModel->setCity(true);
            }
        }
        if(this->buildFarm)
        {
            if(!regionModel->hasFarm() && regionModel->getTribes() >= 2 && regionModel->hasForest())
            {
                regionModel->setTribes(regionModel->getTribes() - 2);
                regionModel->setForest(false);
                regionModel->setFarm(true);
            }
        }
    }

    return this;
}

Instruction *MainPhaseInstruction::triggerDone()
{
    if(this->buildCity || this->buildFarm)
    {
        this->boardModel->disableButtons();
        this->boardModel->enableMainPhaseButtons();
        this->buildCity = false;
        this->buildFarm = false;
        return this;
    }

    if(!this->initialized)
    {
        this->boardModel->clearMessages();
        this->boardModel->sendMessage("Main Phase:");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("You can do any amount of actions by clicking on any of the buttons.");
        this->boardModel->sendMessage("If clicked once, the action is first explained before applying it.");
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("When you are Done, press Done.");
        this->boardModel->sendMessage(" ");
        this->boardModel->enableMainPhaseButtons();
        this->initialized = true;
        return this;
    }

    if(this->initialized)
    {
        this->boardModel->disableButtons();
        this->boardModel->enableDoneButton();
        Instruction *next = new UpkeepInstruction(this->boardModel);
        next->initInstruction();
        return next;
    }

    return this;
}

Instruction *MainPhaseInstruction::triggerBuildCity()
{
    this->buildCity = true;
    this->boardModel->sendMessage("Build City:");
    this->boardModel->sendMessage("Choose a region without a city.");
    this->boardModel->sendMessage("Decimate 4 Tribes on it to build a city with 1 AV.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();

    return this;
}

Instruction *MainPhaseInstruction::triggerBuildFarm()
{
    this->buildFarm = true;
    this->boardModel->sendMessage("Build Farm:");
    this->boardModel->sendMessage("Choose a region without a farm and with a forest.");
    this->boardModel->sendMessage("Decimate 2 Tribes and the forest to build a farm.");
    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->enableDoneButton();

    return this;
}
