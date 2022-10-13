#include "rpgclass.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>

RPGClass::RPGClass(QObject *parent) : QObject(parent)
{

}

QByteArray RPGClass::to_json()
{
    QJsonObject current_class;

    current_class.insert("name", this->name);
    current_class.insert("id", this->id);
    current_class.insert("position", this->position);


    QJsonArray element_r, state_r, weapon_s, armor_s;

    for (int i = 0; i < element_ranks.length(); i++)
        element_r.append(QJsonValue(this->element_ranks.at(i)));
    for (int i = 0; i < state_ranks.length(); i++)
        state_r.append(QJsonValue(this->state_ranks.at(i)));
    for (int i = 0; i < weapon_set.length(); i++)
        weapon_s.append(QJsonValue(this->weapon_set.at(i)));
    for (int i = 0; i < armor_set.length(); i++)
        armor_s.append(QJsonValue(this->armor_set.at(i)));

    current_class.insert("element_ranks", element_r);
    current_class.insert("state_ranks", state_r);
    current_class.insert("weapon_set", weapon_s);
    current_class.insert("armor_set", armor_s);

    QJsonArray learning1, learning2;

    for (int i = 0; i < this->learnings_level.length(); i++)
    {
        learning1.append(QJsonValue(this->learnings_level.at(i)));
        learning2.append(QJsonValue(this->learnings_skill_id.at(i)));
    }
    current_class.insert("learnings_level", learning1);
    current_class.insert("learnings_skill_id", learning2);



    QJsonDocument doc(current_class);

    return doc.toJson();
}

QByteArray RPGClass::to_hash()
{
    QCryptographicHash hash(QCryptographicHash::Algorithm::Sha256);
    hash.addData(this->to_json());

    return hash.result().toHex();
}
