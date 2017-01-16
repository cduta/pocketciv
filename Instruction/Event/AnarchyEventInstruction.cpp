#include "AnarchyEventInstruction.hpp"

#include "Instruction/EndGameInstruction.hpp"
#include "Instruction/EndOfEraInstruction.hpp"
#include "Common.hpp"

AnarchyEventInstruction::AnarchyEventInstruction(BoardModel *boardModel, Instruction *nextInstruction, const Event *event)
    : EventInstruction(boardModel, nextInstruction, event), step(0), cityAVReduceLeft(0), cityAVReduceTotal(2), tribeReduceLeft(0), tribeReduceTotal(0)
{
    this->nextInstruction->setKeepInstruction(true);
    this->possibleRegions = this->boardModel->getRegions().values();
}

void AnarchyEventInstruction::initInstruction()
{
    this->boardModel->printMessage("ANARCHY:");
    this->boardModel->printMessage("In all Regions with cities, Anarchy reigns.");
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::ORGANIZED_RELIGION))
    {
        this->boardModel->printMessage("Advance (ORGANIZED RELIGION):");
        this->boardModel->printMessage("Instead of all Regions with cities,");
        this->boardModel->printMessage("select 4 regions with OR without cities.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("Press done to continue.");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *AnarchyEventInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->step != -1)
    {
        return this;
    }

    if(regionModel && this->step == 1 && this->boardModel->getSelectedRegions().count() < 4)
    {
        if(button == Qt::LeftButton && !regionModel->isSelected())
        {
            regionModel->setSelected(true);
        }
        else if(button == Qt::RightButton && regionModel->isSelected())
        {
            regionModel->setSelected(false);
        }
    }
    else if(regionModel && this->step == 2 && this->affectedRegions.contains(regionModel))
    {
        if(button == Qt::LeftButton &&
           this->boardModel->getCityAVCount() - this->boardModel->getAllSelectedCityAV() > 1 &&
           regionModel->getSelectedCityAV() < regionModel->getCityAV() &&
           this->cityAVReduceLeft > 0)
        {
            regionModel->setSelectedCityAV(regionModel->getSelectedCityAV() + 1);
            this->cityAVReduceLeft--;
            this->boardModel->printMessage(QString("Reduce City AV in Region %1 by %2. (%3/%4 left)")
                                           .arg(regionModel->getRegion())
                                           .arg(regionModel->getSelectedCityAV())
                                           .arg(this->cityAVReduceLeft)
                                           .arg(this->cityAVReduceTotal));
        }
        else if(button == Qt::RightButton && regionModel->getSelectedCityAV() > 0 && this->cityAVReduceLeft < this->cityAVReduceTotal)
        {
            regionModel->setSelectedCityAV(regionModel->getSelectedCityAV() - 1);
            this->cityAVReduceLeft++;
            this->boardModel->printMessage(QString("Reduce City AV in Region %1 by %2. (%3/%4 left)")
                                           .arg(regionModel->getRegion())
                                           .arg(regionModel->getSelectedCityAV())
                                           .arg(this->cityAVReduceLeft)
                                           .arg(this->cityAVReduceTotal));
        }

        if(this->cityAVReduceLeft == 0)
        {
            this->boardModel->printMessage("You can now press done, to apply your choice of City AV reduction.");
        }
    }
    else if(regionModel && this->step == 3 && this->boardModel->getAllSelectedTribes() < this->tribeReduceTotal)
    {
        if(button == Qt::LeftButton &&
           regionModel->getSelectedTribes() < regionModel->getTribes() &&
           this->tribeReduceLeft > 0)
        {
            regionModel->setSelectedTribes(regionModel->getSelectedTribes() + 1);
            this->tribeReduceLeft--;
            this->boardModel->printMessage(QString("Reduce Tribes in Region %1 by %2. (%3/%4 left)")
                                           .arg(regionModel->getRegion())
                                           .arg(regionModel->getSelectedTribes())
                                           .arg(this->tribeReduceLeft)
                                           .arg(this->tribeReduceTotal));
        }
        else if(button == Qt::RightButton &&
                regionModel->getSelectedTribes() > 0 &&
                this->tribeReduceLeft < this->tribeReduceTotal)
        {
            regionModel->setSelectedTribes(regionModel->getSelectedTribes() - 1);
            this->tribeReduceLeft++;
            this->boardModel->printMessage(QString("Reduce Tribes in Region %1 by %2. (%3/%4 left)")
                                           .arg(regionModel->getRegion())
                                           .arg(regionModel->getSelectedTribes())
                                           .arg(this->tribeReduceLeft)
                                           .arg(this->tribeReduceTotal));
        }

        if(this->tribeReduceLeft == 0)
        {
            this->boardModel->printMessage("You can now press done, to apply your choice of tribe reduction.");
        }
    }

    return this;
}

Instruction *AnarchyEventInstruction::triggerDone()
{
    if(this->step == 0 && this->boardModel->hasAdvanceAquired(AdvanceModel::ORGANIZED_RELIGION))
    {
        if(this->boardModel->getSelectedRegions().count() == 4)
        {
            this->boardModel->printMessage(QString("Affected Regions are %1.").arg(Common::listUpRegions(this->possibleRegions)));
            this->step++;
        }
        else
        {
            this->boardModel->printMessage(QString("You still have to select %1 regions.").arg(4 - this->possibleRegions.count()));
        }
    }

    if(this->step == 1)
    {
        this->step++;
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::LAW))
        {
            this->boardModel->printMessage("Advance (LAW):");
            this->boardModel->printMessage("In every region with a city, decimate as many tribes");
            this->boardModel->printMessage("as the City AV of that city.");
            this->boardModel->printMessage(" ");

            foreach(RegionModel *regionModel, this->possibleRegions)
            {
                if(regionModel->hasCity())
                {
                    int tribes = regionModel->getTribes();
                    int cityAV = regionModel->getCityAV();

                    regionModel->decimateTribes(cityAV);

                    this->boardModel->printMessage(QString("In region %1 there have been %2 tribes decimated.")
                                                  .arg(regionModel->getRegion())
                                                  .arg(tribes - regionModel->getTribes()));

                    if(tribes > 0)
                    {
                        this->affectedRegions.append(regionModel);
                    }
                }
            }
        }
        else
        {
            int decimateTribes = 3;

            this->boardModel->printMessage(QString("In every region with a city, reduce 1 City AV and decimate 3 tribes."));
            this->boardModel->printMessage("This is repeated in every region, until the tribes are");
            this->boardModel->printMessage("lower than the City AV or the City AV is 1 in each region.");
            this->boardModel->printMessage(" ");

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::LITERACY))
            {
                decimateTribes += 2;
                this->boardModel->printMessage("Advance (LITERACY):");
                this->boardModel->printMessage("Decimate an additional 2 tribes in every region affected by ANARCHY.");
                this->boardModel->printMessage(" ");
            }

            foreach(RegionModel *regionModel, this->possibleRegions)
            {
                if(regionModel->hasCity() && regionModel->getCityAV() > 0)
                {
                    int cityAV = regionModel->getCityAV();
                    int tribes = regionModel->getTribes();

                    do
                    {
                        regionModel->setCityAV(regionModel->getCityAV()-1);
                        regionModel->setTribes(regionModel->getTribes()-decimateTribes);
                    }
                    while(regionModel->getCityAV() > 1 &&
                          regionModel->getCityAV() <= regionModel->getTribes());


                    int decimatedTribes = tribes-regionModel->getTribes();

                    QString tribeString = "tribe";

                    if(decimatedTribes > 1)
                    {
                        tribeString = "tribes";
                    }

                    this->boardModel->printMessage(QString("In region %1 there have been %2 City AV reduced and %3 %4 decimated.")
                                                  .arg(regionModel->getRegion())
                                                  .arg(cityAV-regionModel->getCityAV())
                                                  .arg(decimatedTribes)
                                                  .arg(tribeString));
                    this->boardModel->printMessage(" ");

                    if(cityAV > 0 || tribes > 0)
                    {
                        this->affectedRegions.append(regionModel);
                    }
                }
            }

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::MACHINING))
            {
                this->boardModel->printMessage("Advance (MACHINING):");
                this->boardModel->printMessage(QString("Reduce %1 additional City AV from any cities of the affected regions.").arg(this->cityAVReduceTotal));
                this->boardModel->printMessage(QString("The affected regions are %1.").arg(Common::listUpRegions(this->affectedRegions)));

                this->boardModel->printMessage(" ");
                int cityAVCount = 0;

                foreach(RegionModel *regionModel, this->affectedRegions)
                {
                    cityAVCount += regionModel->getCityAV();
                }

                if(cityAVCount == 0)
                {
                    this->boardModel->printMessage("There is no City AV in the affected regions left.");
                    this->boardModel->printMessage("Therefore nothing further occurs.");
                    this->boardModel->printMessage(" ");
                }
                else if(cityAVCount == 1 || cityAVCount == 2)
                {
                    this->boardModel->printMessage(QString("There is in total %1 City AV left in the affected regions.").arg(cityAVCount));
                    this->boardModel->printMessage("Therefore any City AV has been reduced to 0.");
                    this->boardModel->printMessage(" ");

                    foreach(RegionModel *regionModel, this->affectedRegions)
                    {
                        regionModel->setCityAV(0);
                    }
                }
                else
                {
                    this->cityAVReduceLeft = this->cityAVReduceTotal;
                    return this;
                }
            }
        }
    }

    if(this->step == 2)
    {
        if(this->cityAVReduceLeft > 0)
        {
            this->boardModel->printMessage(QString("You still have to reduce %1/%2 City AV in the affected regions.").arg(this->cityAVReduceLeft).arg(this->cityAVReduceTotal));
            return this;
        }
        else
        {
            this->step++;

            if(this->boardModel->hasAdvanceAquired(AdvanceModel::SLAVE_LABOR))
            {
                this->boardModel->printMessage("Advance (SLAVE LABOR):");
                this->boardModel->printMessage("Draw a card and reduce the RED CIRCLE amount of tribes in the entire empire.");
                this->boardModel->printMessage(" ");

                this->tribeReduceTotal = this->boardModel->drawCard()->getShapeNumbers().value(Event::RED_CIRCLE);

                this->boardModel->printMessage(QString("Choose %1 tribes in the Empire and decimate them.").arg(this->tribeReduceTotal));
                this->boardModel->printMessage(" ");

                int tribeAmount = this->boardModel->getTribeCount();

                if(tribeAmount <= this->tribeReduceTotal)
                {
                    this->boardModel->printMessage(QString("Because only %1 tribes are left in the entire empire,").arg(tribeAmount));
                    this->boardModel->printMessage("you do not choos and all tribes are decimated.");

                    foreach(RegionModel *regionModel, this->boardModel->getRegions().values())
                    {
                        regionModel->setTribes(0);
                    }

                    if(this->boardModel->getTribeCount() == 0)
                    {
                        Instruction *next = new EndGameInstruction(this->boardModel, true);
                        next->initInstruction();
                        return next;
                    }
                }
                else
                {
                    this->tribeReduceLeft = this->tribeReduceTotal;
                    return this;
                }

                POCKET_CIV_END_OF_ERA_CHECK
            }
        }
    }

    if(this->step == 3)
    {
        if(this->tribeReduceLeft > 0)
        {
            this->boardModel->printMessage(QString("You still have to reduce %1/%2 Tribes in the Empire.").arg(this->tribeReduceLeft).arg(this->tribeReduceTotal));
            return this;
        }
        else
        {
            this->step++;
        }
    }


    return this->endEvent();
}

