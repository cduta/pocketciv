#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <QObject>
#include <QDialog>

// FIXME: Remove macro and replace its usage.
#define POCKET_CIV_END_OF_ERA_CHECK \
    if(this->boardModel->isEndOfEra()) \
    { \
        Instruction *next = new EndOfEraInstruction(this->boardModel, this); \
        next->initInstruction(); \
        return next; \
    }

class Instruction : public QObject
{
    Q_OBJECT

protected:
    bool keep;

public:
    Instruction(QObject *parent = 0);
    virtual ~Instruction();

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton, int, int);
    virtual Instruction *triggerDone();
    virtual Instruction *getFollowingInstruction();

    virtual Instruction *triggerBuildCity();
    virtual Instruction *triggerBuildFarm();
    virtual Instruction *triggerExpedition();
    virtual Instruction *triggerAquireAdvance();
    virtual Instruction *triggerBuildWonder();

    bool keepInstruction();

    void setKeepInstruction(bool keep);
};

class NoInstruction : public Instruction
{

public:
    NoInstruction()
        : Instruction()
    {}
};

#endif // INSTRUCTION_HPP
