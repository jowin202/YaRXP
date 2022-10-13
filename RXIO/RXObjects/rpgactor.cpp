#include "rpgactor.h"
#include "rpgsystem.h"

#include <QCryptographicHash>

RPGActor::RPGActor(QObject *parent) : QObject(parent)
{

}

QByteArray RPGActor::to_json()
{
    QJsonObject actor;

    actor.insert("battler_name", this->battler_name);
    actor.insert("character_name", this->character_name);
    actor.insert("name", this->name);

    actor.insert("id", this->id);
    actor.insert("initial_level", this->initial_level);
    actor.insert("final_level", this->final_level);
    actor.insert("character_hue", this->character_hue);
    actor.insert("battler_hue", this->battler_hue);
    actor.insert("class_id", this->class_id);
    actor.insert("exp_basis", this->exp_basis);
    actor.insert("exp_inflation", this->exp_inflation);
    actor.insert("weapon_id", this->weapon_id);

    actor.insert("armor1_id", this->armor1_id);
    actor.insert("armor2_id", this->armor2_id);
    actor.insert("armor3_id", this->armor3_id);
    actor.insert("armor4_id", this->armor4_id);

    actor.insert("armor1_fix", this->armor1_fix);
    actor.insert("armor2_fix", this->armor2_fix);
    actor.insert("armor3_fix", this->armor3_fix);
    actor.insert("armor4_fix", this->armor4_fix);
    actor.insert("weapon_fix", this->weapon_fix);

    QJsonArray maxhp, maxsp, str, dex, agi, int_var;

    for (int i = 0; i < 100; i++)
    {
        maxhp.append(QJsonValue(this->maxhp.at(i)));
        maxsp.append(QJsonValue(this->maxsp.at(i)));
        str.append(QJsonValue(this->str.at(i)));
        dex.append(QJsonValue(this->dex.at(i)));
        agi.append(QJsonValue(this->agi.at(i)));
        int_var.append(QJsonValue(this->int_var.at(i)));
    }
    actor.insert("maxhp", maxhp);
    actor.insert("maxsp", maxhp);

    actor.insert("str", str);
    actor.insert("dex", dex);
    actor.insert("agi", agi);
    actor.insert("int", int_var);


    QJsonDocument doc(actor);
    return doc.toJson();
}

QByteArray RPGActor::to_hash()
{
    QCryptographicHash hash(QCryptographicHash::Algorithm::Sha256);
    hash.addData(this->to_json());

    return hash.result().toHex();
}

