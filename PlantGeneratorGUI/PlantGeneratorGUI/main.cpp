#include "SwitchWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SwitchWindow w;
    w.show();

    return a.exec();
}
