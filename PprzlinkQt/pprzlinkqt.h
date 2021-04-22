#ifndef PPRZLINKQT_H
#define PPRZLINKQT_H

#include <QObject>
#include "message_definition.h"
#include "message.h"

class IvyQt;

class PprzlinkQt : public QObject
{
    Q_OBJECT
public:
    explicit PprzlinkQt(QString filename, QString ivy_name, QString msg_ready, QObject *parent = nullptr);
    void start(QString domain, int port);
    void stop();
    int bind(QString msg_name, QObject* context, std::function<void(QString sender, Message)>);
    int bind(QString msg_name, std::function<void(QString sender, Message)>);
    void unBind(int bindId);
    void sendMessage(Message &msg);
    MessageDefinition getDefinition(QString msg_name);

signals:

private:
    static QString stringOfMessage(Message &msg);
    void parse_definitions(QString filename);
    QList<MessageDefinition> message_definitions;
    //QMap<int /*bindId*/, MessageDefinition> bindings;

    IvyQt* ivyqt;

};

#endif // PPRZLINKQT_H
