#include "injector.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    injector w;
    w.show();
    return a.exec();
}
