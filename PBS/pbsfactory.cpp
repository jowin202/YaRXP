#include "pbsfactory.h"

PBSFactory::PBSFactory()
{

}

QJsonObject PBSFactory::create_symbol(QString value)
{
        QJsonObject symbol;
        symbol.insert("RXClass", "symbol");
        symbol.insert("symbol_value", value);
        return symbol;
}

QJsonObject PBSFactory::create_empty_dict()
{
    QJsonObject dict;
    dict.insert("RXClass", "dictionary");
    dict.insert("dict_list", QJsonArray());
    return dict;
}

QJsonArray PBSFactory::create_encounter(int map, int version)
{
    QJsonArray result;

    result.append(this->create_symbol(QString("%1_%2").arg(map).arg(version)));

    QJsonObject obj;
    obj.insert("@id", this->create_symbol(QString("%1_%2").arg(map).arg(version)));
    obj.insert("@map", map);
    obj.insert("@version", version);
    obj.insert("RXClass", "GameData::Encounter");

    obj.insert("@step_chances", this->create_empty_dict());
    obj.insert("@types", this->create_empty_dict());
    result.append(obj);

    return result;
}

QJsonArray PBSFactory::encounter_add_type(QJsonArray encounter, QString type, int step_chance)
{
    QJsonObject encounter_obj = encounter.at(1).toObject();
    QJsonObject step_chances = encounter_obj.value("@step_chances").toObject();
    QJsonArray dict_list = step_chances.value("dict_list").toArray();
    QJsonArray dict_list_element;
    dict_list_element.append(this->create_symbol(type));
    dict_list_element.append(step_chance);
    dict_list.append(dict_list_element);
    step_chances.insert("dict_list",dict_list);
    encounter_obj.insert("@step_chances",step_chances);

    QJsonObject types = encounter_obj.value("@types").toObject();
    dict_list = types.value("dict_list").toArray();
    QJsonArray type_element;
    type_element.append(this->create_symbol(type));
    type_element.append(QJsonArray());
    dict_list.append(type_element);
    types.insert("dict_list",dict_list);
    encounter_obj.insert("@types",types);

    encounter.removeAt(1);
    encounter.insert(1, encounter_obj);
    return encounter;
}

QJsonArray PBSFactory::encounter_add_mon(QJsonArray encounter, QString type, QString name, int prob, int from_lv, int to_lv)
{
    QJsonObject encounter_obj = encounter.at(1).toObject();
    QJsonObject types = encounter_obj.value("@types").toObject();
    QJsonArray dict_list = types.value("dict_list").toArray();

    for (int i = 0; i < dict_list.count(); i++)
    {
        if (dict_list.at(i).toArray().at(0).toObject().value("symbol_value").toString() == type)
        {
            QJsonArray type_list = dict_list.at(i).toArray();
            QJsonArray mon_list = type_list.at(1).toArray();
            QJsonArray new_mon;
            new_mon.append(prob);
            new_mon.append(this->create_symbol(name));
            new_mon.append(from_lv);
            new_mon.append(to_lv);
            mon_list.append(new_mon);
            type_list.removeAt(1);
            type_list.insert(1, mon_list);
            dict_list.removeAt(i);
            dict_list.insert(i, type_list);
            types.insert("dict_list", dict_list);
            encounter_obj.insert("@types", types);
            encounter.removeAt(1);
            encounter.insert(1, encounter_obj);
            break;
        }
    }
    return encounter;
}

QJsonArray PBSFactory::create_trainer_type(QString id, QString real_name, int base_money, int skill_level, int gender, QString intro_bgm, QString battle_bgm, QString victory_bgm)
{
    QJsonObject id_obj = this->create_symbol(id);

    QJsonObject basic_obj;
    basic_obj.insert("@id", id_obj);
    basic_obj.insert("@gender", gender);
    basic_obj.insert("@base_money", base_money);
    basic_obj.insert("@skill_level", skill_level);
    basic_obj.insert("@flags", QJsonArray());
    basic_obj.insert("@battle_BGM", (battle_bgm == "" ? QJsonValue::Null : QJsonValue(battle_bgm)));
    basic_obj.insert("@intro_BGM", (intro_bgm == "" ? QJsonValue::Null : QJsonValue(intro_bgm)));
    basic_obj.insert("@victory_BGM", (victory_bgm == "" ? QJsonValue::Null : QJsonValue(victory_bgm)));
    basic_obj.insert("@real_name", real_name);
    basic_obj.insert("RXClass", "GameData::TrainerType");

    QJsonArray result;
    result.append(id_obj);
    result.append(basic_obj);
    return result;
}
