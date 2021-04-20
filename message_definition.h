#ifndef MESSAGEDEFINITION_H
#define MESSAGEDEFINITION_H

#include <QString>
#include <QtXml>

class MessageDefinition
{
public:
    MessageDefinition(QDomElement msg_elt, QString class_name, uint8_t class_id);

    QString getName() {return name;}
    uint8_t getId() {return id;}
    QString getClassName() {return class_name;}
    uint8_t getClassId() {return class_id;}
    QString getDescription() {return description;}

private:
    QString class_name;
    uint8_t class_id;
    QString name;
    uint8_t id;
    QString description;

    //fields

};

#endif // MESSAGEDEFINITION_H
