#include "rpgeventpagecondition.h"

RPGEventPageCondition::RPGEventPageCondition(QObject *parent) : QObject(parent)
{

}

void RPGEventPageCondition::setParameter(QString param, int val)
{
    if (param == "@switch1_id")
        this->switch1_id = val;
    else if (param == "@switch2_id")
        this->switch1_id = val;
    else if (param == "@variable_id")
        this->variable_id = val;
    else if (param == "@variable_value")
        this->variable_value = val;
    else if (param == "@self_switch_ch")
        this->self_switch_ch = val;
}

void RPGEventPageCondition::setParameter(QString param, bool val)
{
    if (param == "@self_switch_valid")
        this->self_switch_valid = val;
    else if (param == "@variable_valid")
        this->variable_valid = val;
    else if (param == "@switch1_valid")
        this->switch1_valid = val;
    else if (param == "@switch2_valid")
        this->switch2_valid = val;
}
