#ifndef MESSAGE_H
#define MESSAGE_H

#include <any>
#include "message_definition.h"


class Message
{
public:
    Message(MessageDefinition definition);
    Message(MessageDefinition definition, QStringList values);

    bool isComplete();
    QString getSender() { return sender;}
    void setSender(QString s) {
        sender = s;
    }
    MessageDefinition getDefinition() { return definition;}

    template<
      typename T,
      typename E = typename std::enable_if<std::is_arithmetic<T>::value>::type
    >
    void getField(QString field_name, T& arg){
        if(!fields.contains(field_name)) {
            throw std::runtime_error("No such field: \"" + field_name.toStdString() + "\"!");
        }
        arg = std::any_cast<T>(fields[field_name]);
     }

    QString getField(QString field_name);

    void getField(QString field_name, QString& arg);

    template<typename T>
    void addField(QString name, T value);

    MessageDefinition messageDefinition() const {return definition;}

private:
    MessageDefinition definition;
    QString sender;
    QMap<QString, std::any> fields;
};

#endif // MESSAGE_H
