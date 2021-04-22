#include <QApplication>
#include "pprzlinkqt.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PprzlinkQt pp("/home/fabien/paparazzi/var/messages.xml", "testPprzLinkQt", "testPprzLinkQt ready");

    pp.start("127.255.255.255", 2010);

    pp.bind("FLIGHT_PARAM", [](QString sender, Message msg) {
        //qDebug() << sender;
        QString ac_id;
        float alt;
        msg.getField("ac_id", ac_id);
        msg.getField("alt", alt);
        qDebug() << ac_id << alt;

    });

    return a.exec();
}

