#include "pprzlinkqt.h"
#include <QtXml>
#include "message_definition.h"


PprzlinkQt::PprzlinkQt(QString filename, QObject *parent) : QObject(parent)
{
    QDomDocument xmlMessages;
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Error while loading layout file");
    }
    xmlMessages.setContent(&f);
    f.close();

    QDomElement root = xmlMessages.documentElement();

    QString rootTag = root.tagName();

    if (rootTag != "protocol") {
        throw std::runtime_error("Root tag expected to be \"protocol\". Is this a messages file ?");
    }

    for(auto class_elt = root.firstChildElement();
        !class_elt.isNull();
        class_elt = class_elt.nextSiblingElement()) {

        QString class_name = class_elt.attribute("name");
        uint8_t class_id = class_elt.attribute("id").toUInt();
        qDebug() << class_name << class_id;

        for(auto msg_elt = class_elt.firstChildElement();
            !msg_elt.isNull();
            msg_elt = msg_elt.nextSiblingElement()) {

            MessageDefinition def(msg_elt, class_name, class_id);
            qDebug() << def.getName() << def.getId() << def.getDescription();

        }

    }

}
