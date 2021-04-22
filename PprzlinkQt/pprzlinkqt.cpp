#include "pprzlinkqt.h"
#include "ivyqt.h"
#include <QtXml>


PprzlinkQt::PprzlinkQt(QString filename, QString ivy_name, QString msg_ready, QObject *parent) : QObject(parent)
{
    parse_definitions(filename);
    ivyqt = new IvyQt(ivy_name, msg_ready, this);
}


void PprzlinkQt::parse_definitions(QString filename) {
    QDomDocument xmlMessages;
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Error while loading layout file");
    }
    xmlMessages.setContent(&f);
    f.close();

    QDomElement root = xmlMessages.documentElement();

    QString rootTag = root.tagName();

    if(rootTag != "protocol") {
        throw std::runtime_error("Root tag expected to be \"protocol\". Is this a messages file ?");
    }

    for(auto class_elt = root.firstChildElement();
        !class_elt.isNull();
        class_elt = class_elt.nextSiblingElement()) {

        QString class_name = class_elt.attribute("name");
        uint8_t class_id = class_elt.attribute("id").toUInt();
        //qDebug() << class_name << class_id;

        for(auto msg_elt = class_elt.firstChildElement();
            !msg_elt.isNull();
            msg_elt = msg_elt.nextSiblingElement()) {

            MessageDefinition def(msg_elt, class_name, class_id);
            message_definitions.append(def);
        }

    }
}

void PprzlinkQt::start(QString domain, int port) {
    ivyqt->start(domain, port);
}

void PprzlinkQt::stop() {
    ivyqt->stop();
}

int PprzlinkQt::bind(QString msg_name, std::function<void(QString sender, Message)> callback) {
    return bind(msg_name, nullptr, callback);
}

int PprzlinkQt::bind(QString msg_name, QObject* context,  std::function<void(QString sender, Message)> callback) {
    auto def = getDefinition(msg_name);
    return ivyqt->bindMessage(def.toRegex(), context, [=](Peer* peer, QStringList params) {
        (void)peer;
        QString sender = params.takeFirst();
        Message msg(def, params);
        callback(sender, msg);
    });
}

MessageDefinition PprzlinkQt::getDefinition(QString msg_name) {
    for(auto &def: qAsConst(message_definitions)) {
        if(def.getName() == msg_name) {
            return def;
        }
    }
    throw std::runtime_error("no such message: " + msg_name.toStdString() + "!");
}

void PprzlinkQt::unBind(int bindId) {
    ivyqt->unBindMessage(bindId);
}

void PprzlinkQt::sendMessage(Message &msg) {
    assert(msg.isComplete());
    auto message = stringOfMessage(msg);
    qDebug() << message;
    ivyqt->send(message);
}

QString PprzlinkQt::stringOfMessage(Message &msg) {
    QString str = msg.getSender() + " " + msg.getDefinition().getName();

    for(auto fdef: msg.getDefinition().getFields()) {
        str += " " + msg.getField(fdef.name);
    }

    //TODO

    return str;
}
