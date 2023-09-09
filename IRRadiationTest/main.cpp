#include "IRRadiationTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IRRadiationTest w;
    w.show();
    return a.exec();
}
