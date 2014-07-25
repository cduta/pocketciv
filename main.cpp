#include "PocketCivMain.hpp"
#include <QApplication>

#include <QList>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PocketCivMain w;
    w.show();

    return a.exec();
}
