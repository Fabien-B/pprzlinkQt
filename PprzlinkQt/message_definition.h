#ifndef MESSAGEDEFINITION_H
#define MESSAGEDEFINITION_H

#include <QString>
#include <QtXml>
#include <QRegularExpression>

enum Type {
    CHAR,
    U8,
    U16,
    U32,
    I8,
    I16,
    I32,
    FLOAT,
    ARRAY,      //treat it has string for now
    STRING,
    UNKNOWN,
};

struct FieldDef {
    QString name;
    Type type;
    QString format;
    QString unit;
    QString values;
    QString alt_unit;
    QString alt_unit_coef;
    QString description;
};

class MessageDefinition
{
public:
    MessageDefinition(QDomElement msg_elt, QString class_name, uint8_t class_id);

    QString getName() const {return name;}
    uint8_t getId() const {return id;}
    QString getClassName() const {return class_name;}
    uint8_t getClassId() const {return class_id;}
    QString getDescription() const {return description;}
    QList<FieldDef> getFields() {return fields;}
    QString toRegex() const;


private:

    static Type typeFromString(QString type);
    static QString regexOfField(FieldDef field);

    QString class_name;
    uint8_t class_id;
    QString name;
    uint8_t id;
    QString description;

    QList<FieldDef> fields;

};

#endif // MESSAGEDEFINITION_H
