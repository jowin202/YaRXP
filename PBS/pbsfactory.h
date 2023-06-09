#ifndef PBSFACTORY_H
#define PBSFACTORY_H

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>

class PBSFactory
{
public:
    PBSFactory();

    QJsonObject create_symbol(QString value);
    QJsonObject create_empty_dict();

    /*
    PBSFactory factory;
    QJsonArray encounter = factory.create_encounter(2,0);

    encounter = factory.encounter_add_type(encounter, "Water", 2);
    encounter = factory.encounter_add_mon(encounter, "Water", "TENTACOOL", 60,14,19);
    encounter = factory.encounter_add_mon(encounter, "Water", "MANTYKE", 30,15,16);
    encounter = factory.encounter_add_mon(encounter, "Water", "REMORAID", 10,14,16);

    encounter = factory.encounter_add_type(encounter, "OldRod", 0);
    encounter = factory.encounter_add_mon(encounter, "OldRod", "MAGIKARP", 100,16,19);

    encounter = factory.encounter_add_type(encounter, "GoodRod", 0);
    encounter = factory.encounter_add_mon(encounter, "GoodRod", "BARBOACH", 60,17,18);
    encounter = factory.encounter_add_mon(encounter, "GoodRod", "KRABBY", 20,15,16);
    encounter = factory.encounter_add_mon(encounter, "GoodRod", "SHELLDER", 20,16,19);

    encounter = factory.encounter_add_type(encounter, "SuperRod", 0);
    encounter = factory.encounter_add_mon(encounter, "SuperRod", "CHINCHOU", 40,17,19);
    encounter = factory.encounter_add_mon(encounter, "SuperRod", "QWILFISH", 40,16,19);
    encounter = factory.encounter_add_mon(encounter, "SuperRod", "CORSOLA", 15,15,18);
    encounter = factory.encounter_add_mon(encounter, "SuperRod", "STARYU", 5,15,17);
    */
    QJsonArray create_encounter(int map, int version = 0);
    QJsonArray encounter_add_type(QJsonArray encounter, QString type, int step_chance);
    QJsonArray encounter_add_mon(QJsonArray encounter, QString type, QString name, int prob, int from_lv, int to_lv);




    QJsonArray create_trainer_type(QString id, QString real_name, int base_money, int skill_level, int gender, QString intro_bgm, QString battle_bgm, QString victory_bgm);

};

#endif // PBSFACTORY_H
