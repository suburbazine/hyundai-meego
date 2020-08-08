#include <QtGui/QApplication>
#include "AutoTestWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AutoTestWidget w;

    w.resize(1280, 720);

    //w.show();
    return a.exec();
}
