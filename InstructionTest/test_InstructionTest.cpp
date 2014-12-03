#include <QString>
#include <QtTest>

class InstructionTest : public QObject
{
    Q_OBJECT

public:
    InstructionTest();

private Q_SLOTS:
    void instructionTestCase();
};

InstructionTest::InstructionTest()
{

}

void InstructionTest::instructionTestCase()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(InstructionTest)

#include "test_InstructionTest.moc"
