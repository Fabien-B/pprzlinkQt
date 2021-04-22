#include "message.h"

Message::Message(MessageDefinition definition): definition(definition)
{

}

Message::Message(MessageDefinition definition, QStringList values) : definition(definition)
{
    assert(definition.getFields().size() == values.size());

    auto defs = definition.getFields();
    for(int i=0; i<defs.size(); i++) {
        auto fieldDef = defs[i];
        if(fieldDef.type == Type::U8) {
            uint8_t val = values[i].toUInt();
            addField(fieldDef.name, val);
        } else if(fieldDef.type == Type::I8) {
            int8_t val = values[i].toInt();
            addField(fieldDef.name, val);
        } else if(fieldDef.type == Type::U16) {
            uint16_t val = values[i].toUInt();
            addField(fieldDef.name, val);
        } else if(fieldDef.type == Type::I16) {
            int16_t val = values[i].toInt();
            addField(fieldDef.name, val);
        } else if(fieldDef.type == Type::U32) {
            uint32_t val = values[i].toUInt();
            addField(fieldDef.name, val);
        } else if(fieldDef.type == Type::I32) {
            int32_t val = values[i].toInt();
            addField(fieldDef.name, val);
        } else if(fieldDef.type == Type::FLOAT) {
            float val = values[i].toFloat();
            addField(fieldDef.name, val);
        } else if(fieldDef.type == Type::STRING) {
            QString val = values[i];
            addField(fieldDef.name, val);
        } else if(fieldDef.type == Type::ARRAY) {
            QString val = values[i];
            addField(fieldDef.name, val);
        }
    }
}

QString Message::getField(QString field_name) {
    auto field_defs = definition.getFields();

    auto def = std::find_if(field_defs.begin(), field_defs.end(),
                 [=](FieldDef fd){return fd.name == field_name;});
    if(def == field_defs.end()) {
        throw std::runtime_error("No such field: \"" + field_name.toStdString() + "\"!");
    }

    if(def->type == Type::U8) {
        uint8_t val;
        getField(field_name, val);
        return QString::number(val);
    } else if(def->type == Type::I8) {
        int8_t val;
        getField(field_name, val);
        return QString::number(val);
    } else if(def->type == Type::U16) {
        uint16_t val;
        getField(field_name, val);
        return QString::number(val);
    } else if(def->type == Type::I16) {
        int16_t val;
        getField(field_name, val);
        return QString::number(val);
    } else if(def->type == Type::U32) {
        uint32_t val;
        getField(field_name, val);
        return QString::number(val);
    } else if(def->type == Type::I32) {
        int32_t val;
        getField(field_name, val);
        return QString::number(val);
    } else if(def->type == Type::FLOAT) {
        float val;
        getField(field_name, val);
        return QString::number(val);
    } else if(def->type == Type::STRING) {
        QString val;
        getField(field_name, val);
        return val;
    } else if(def->type == Type::ARRAY) {
        throw std::runtime_error("arrays not done yet!");
    }

    throw std::runtime_error("unknown type!");
}

template<typename T>
void Message::addField(QString name, T value) {

    auto field_defs = definition.getFields();

    auto def = std::find_if(field_defs.begin(), field_defs.end(),
                 [=](FieldDef fd){return fd.name == name;});
    if(def == field_defs.end()) {
        throw std::runtime_error("No such field: \"" + name.toStdString() + "\"!");
    }

    bool type_ok = false;
    if(def->type == Type::U8) {
        type_ok = std::is_same_v<T, uint8_t >;
    } else if(def->type == Type::U16) {
        type_ok = std::is_same_v<T, uint16_t >;
    } else if(def->type == Type::U32) {
        type_ok =  std::is_same_v<T, uint32_t >;
    } else if(def->type == Type::I8) {
        type_ok = std::is_same_v<T, int8_t >;
    } else if(def->type == Type::I16) {
        type_ok = std::is_same_v<T, int16_t >;
    } else if(def->type == Type::I32) {
        type_ok = std::is_same_v<T, int32_t >;
    } else if(def->type == Type::FLOAT) {
        type_ok = std::is_same_v<T, float >;
    } else if(def->type == Type::STRING) {
        type_ok = std::is_same_v<T, QString >;
    } else if(def->type == Type::CHAR) {
        type_ok = std::is_same_v<T, char >;
    }

    if(!type_ok) {
        throw std::runtime_error("Bad type!");
    }

    fields[name] = std::make_any<T>(value);
}

void Message::getField(QString field_name, QString& arg){
    if(!fields.contains(field_name)) {
        throw std::runtime_error("no such field!");
    }
    arg = std::any_cast<QString>(fields[field_name]);
}

bool Message::isComplete() {
    // fields are checked at insertion,
    // so just check if the number of fields match
    return definition.getFields().size() == fields.size();
}

