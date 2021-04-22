#include "message.h"

Message::Message(MessageDefinition definition): definition(definition)
{

}

Message::Message(MessageDefinition definition, QStringList values) : definition(definition)
{
    assert(definition.getFields().size() == values.size());

    for(int i=0; i<definition.getFields().size(); i++) {
        auto fieldDef = definition.getFields()[i];
        if(fieldDef.type == Type::U8) {
            uint8_t val = values[i].toUInt();
            addField(val);
        } else if(fieldDef.type == Type::I8) {
            int8_t val = values[i].toInt();
            addField(val);
        } else if(fieldDef.type == Type::U16) {
            uint16_t val = values[i].toUInt();
            addField(val);
        } else if(fieldDef.type == Type::I16) {
            int16_t val = values[i].toInt();
            addField(val);
        } else if(fieldDef.type == Type::U32) {
            uint32_t val = values[i].toUInt();
            addField(val);
        } else if(fieldDef.type == Type::I32) {
            int32_t val = values[i].toInt();
            addField(val);
        } else if(fieldDef.type == Type::FLOAT) {
            float val = values[i].toFloat();
            addField(val);
        } else if(fieldDef.type == Type::STRING) {
            QString val = values[i];
            addField(val);
        } else if(fieldDef.type == Type::ARRAY) {
            QString val = values[i];
            addField(val);
        }
    }
}

template<typename T>
void Message::addField(T value) {
    fields.append(std::make_any<T>(value));
}

