#ifndef MESSAGE_H
#define MESSAGE_H

#include <any>
#include "message_definition.h"


class Message
{
public:
    Message(MessageDefinition definition);
    Message(MessageDefinition definition, QStringList values);

    template<
      typename T,
      typename E = typename std::enable_if<std::is_arithmetic<T>::value>::type
    >
    void getField(QString field_name, T& arg){
         for(int i=0; i < fields.size(); i++) {
             if(definition.getFields()[i].name == field_name) {

                 arg = std::any_cast<T>(fields[i]);
             }
         }
     }

    void getField(QString field_name, QString& arg){
         for(int i=0; i < fields.size(); i++) {
             if(definition.getFields()[i].name == field_name) {
                 arg = std::any_cast<QString>(fields[i]);
             }
         }
     }

    MessageDefinition messageDefinition() const {return definition;}

private:

    template<typename T>
    void addField(T value);

    MessageDefinition definition;
    QList<std::any> fields;
};

#endif // MESSAGE_H
