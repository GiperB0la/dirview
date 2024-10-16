#include "dirview.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dirview window;
    window.show();

    return a.exec();
}
