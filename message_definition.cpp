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
            QString field_description = elt.text();
            QString field_name = elt.attribute("name");
            QString field_type = elt.attribute("type");
            QString format = elt.attribute("format");
            QString unit = elt.attribute("unit");
            QString values = elt.attribute("values");
            QString alt_unit = elt.attribute("alt_unit");
            QString alt_unit_coef = elt.attribute("alt_unit_coef");
            qDebug() << field_name << field_type << format << unit << values << alt_unit << alt_unit_coef;

        }
    }
}
