#include "rpgskill.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCryptographicHash>

RPGSkill::RPGSkill(QObject *parent) : QObject(parent)
{

}


QByteArray RPGSkill::to_json()
{
    QJsonObject skill;

    skill.insert("description", this->description);
    skill.insert("icon_name", this->icon_name);
    skill.insert("name", this->name);

    skill.insert("id", this->id);
    skill.insert("sp_cost", this->sp_cost);
    skill.insert("power", this->power);
    skill.insert("atk_f", this->atk_f);
    skill.insert("eva_f", this->eva_f);


    skill.insert("str", this->str_f);
    skill.insert("dex", this->dex_f);
    skill.insert("agi", this->agi_f);
    skill.insert("int", this->int_f);

    skill.insert("hit", this->hit);
    skill.insert("pdef", this->pdef_f);
    skill.insert("mdef", this->mdef_f);
    skill.insert("variance", this->variance);

    skill.insert("scope", this->scope);
    skill.insert("occasion", this->occasion);
    skill.insert("animation1_id", this->animation1_id);
    skill.insert("animation2_id", this->animation2_id);

    QJsonArray element, plus, minus;

    for (int i = 0; i < element_set.length(); i++)
        element.append(QJsonValue(this->element_set.at(i)));
    for (int i = 0; i < plus_state_set.length(); i++)
        plus.append(QJsonValue(this->plus_state_set.at(i)));
    for (int i = 0; i < minus_state_set.length(); i++)
        minus.append(QJsonValue(this->minus_state_set.at(i)));

    skill.insert("element_set", element);
    skill.insert("plus_state_set", plus);
    skill.insert("minus_state_set", minus);



    QJsonDocument doc(skill);
    return doc.toJson();
}

QByteArray RPGSkill::to_hash()
{
    QCryptographicHash hash(QCryptographicHash::Algorithm::Sha256);
    hash.addData(this->to_json());

    return hash.result().toHex();
}
