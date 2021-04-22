#include "message_definition.h"


MessageDefinition::MessageDefinition(QDomElement msg_elt, QString class_name, uint8_t class_id):
    class_name(class_name), class_id(class_id)
{
    name = msg_elt.attribute("name");
    id = msg_elt.attribute("id").toUInt();

    for(auto elt = msg_elt.firstChildElement();
        !elt.isNull();
        elt = elt.nextSiblingElement()) {

        if(elt.tagName() == "description") {
            description = elt.text();
        } else if(elt.tagName() == "field") {

            struct FieldDef field = {
                elt.attribute("name"),
                typeFromString(elt.attribute("type")),
                elt.attribute("format"),
                elt.attribute("unit"),
                elt.attribute("values"),
                elt.attribute("alt_unit"),
                elt.attribute("alt_unit_coef"),
                elt.text(),
            };

            fields.append(field);

        }
    }
}

QString MessageDefinition::toRegex() const {
    QString regex = QString("^([^ ]+) %1").arg(name);  // <sender> <MSG_NAME>

    for(auto &f: qAsConst(fields)) {
        regex += QString(" %1").arg(regexOfField(f));
    }
    regex += "$";
    return regex;
}


Type MessageDefinition::typeFromString(QString type) {
    if(type == "uint8") {
        return Type::U8;
    } else if(type == "int8") {
        return Type::I8;
    } else if(type == "uint16") {
        return Type::U16;
    } else if(type == "int16") {
        return Type::I16;
    } else if(type == "uint32") {
        return Type::U32;
    } else if(type == "int32") {
        return Type::I32;
    } else if(type == "float") {
        return Type::FLOAT;
    } else if(type == "string") {
        return Type::STRING;
    } else if(type == "char") {
        return Type::CHAR;
    } else if(type.contains("[") && type.contains("]")) {
        return Type::ARRAY;
    } else {
        return Type::UNKNOWN;
        qDebug() << "Type " << type << "unknown!!!";
    }
}

QString MessageDefinition::regexOfField(FieldDef field) {
    if(field.type == U8 ||
       field.type == U16 ||
       field.type == U32) {
        return "(\\d+)";
    } else if(field.type == I8 ||
              field.type == I16 ||
              field.type == I32) {
        return "(-?\\d+)";
    } else if(field.type == FLOAT) {
        return "(-?\\d+(?:\\.)?(?:\\d)*)";
    } else if(field.type == STRING) {
        return "((?:\"[^\"]+\"|[^ ]+))";
    } else if(field.type == CHAR) {
        return "TODO";
    } else if(field.type == ARRAY) {
        return "TODO";
    } else if(field.type == UNKNOWN) {
        throw std::runtime_error("unknown type");
    } else {
        throw std::runtime_error("very unknown type");
    }

    return "plop";
}
