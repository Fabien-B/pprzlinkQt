#include <QApplication>
#include "pprzlinkqt.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PprzlinkQt pp("/home/fabien/paparazzi/var/messages.xml");

    return a.exec();
}

