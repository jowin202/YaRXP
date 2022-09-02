
#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgclass.h"
#include "datasource.h"



DataSource::DataSource(QObject *parent) : QObject(parent)
{

}

void DataSource::fill_list(QListWidget *list, int type, bool shownum, int chars)
{
    list->clear();

    if (type == ACTORS)
        for (int i = 0; i < system->actor_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->actor_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->actor_list.at(i)->name);
    else if (type == CLASSES)
        for (int i = 0; i < system->classes_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->classes_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->classes_list.at(i)->name);
    else if (type == SKILLS)
        for (int i = 0; i < system->skills_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->skills_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->skills_list.at(i)->name);
    else if (type == ITEMS)
        for (int i = 0; i < system->items_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->items_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->items_list.at(i)->name);
    else if (type == WEAPONS)
        for (int i = 0; i < system->weapons_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->weapons_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->weapons_list.at(i)->name);
    else if (type == ARMORS)
        for (int i = 0; i < system->armors_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->armors_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->armors_list.at(i)->name);
    else if (type == ENEMIES)
        for (int i = 0; i < system->enemies_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->enemies_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->enemies_list.at(i)->name);
    else if (type == TROOPS)
        for (int i = 0; i < system->troops_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->troops_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->troops_list.at(i)->name);
    else if (type == STATES)
        for (int i = 0; i < system->states_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->states_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->states_list.at(i)->name);
    else if (type == ANIMATIONS)
        for (int i = 0; i < system->animation_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->animation_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->animation_list.at(i)->name);
    else if (type == TILESETS)
        for (int i = 0; i < system->tileset_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->tileset_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->tileset_list.at(i)->name);
    else if (type == COMMONEVENTS)
        for (int i = 0; i < system->common_events_list.length(); i++)
            list->addItem(shownum ? QString("%1: " + system->common_events_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->common_events_list.at(i)->name);
}


void DataSource::fill_combo(QComboBox *combo, int type, bool shownum, int chars, int current = -1)
{
    combo->clear();

    if (type == ACTORS)
        for (int i = 0; i < system->actor_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->actor_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->actor_list.at(i)->name);
    else if (type == CLASSES)
        for (int i = 0; i < system->classes_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->classes_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->classes_list.at(i)->name);
    else if (type == SKILLS)
        for (int i = 0; i < system->skills_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->skills_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->skills_list.at(i)->name);
    else if (type == ITEMS)
        for (int i = 0; i < system->items_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->items_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->items_list.at(i)->name);
    else if (type == WEAPONS)
        for (int i = 0; i < system->weapons_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->weapons_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->weapons_list.at(i)->name);
    else if (type == ARMORS)
        for (int i = 0; i < system->armors_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->armors_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->armors_list.at(i)->name);
    else if (type == ENEMIES)
        for (int i = 0; i < system->enemies_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->enemies_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->enemies_list.at(i)->name);
    else if (type == TROOPS)
        for (int i = 0; i < system->troops_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->troops_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->troops_list.at(i)->name);
    else if (type == STATES)
        for (int i = 0; i < system->states_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->states_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->states_list.at(i)->name);
    else if (type == ANIMATIONS)
        for (int i = 0; i < system->animation_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->animation_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->animation_list.at(i)->name);
    else if (type == TILESETS)
        for (int i = 0; i < system->tileset_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->tileset_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->tileset_list.at(i)->name);
    else if (type == COMMONEVENTS)
        for (int i = 0; i < system->common_events_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->common_events_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->common_events_list.at(i)->name);

    if (current >= 0 && combo->count() >= current)
        combo->setCurrentIndex(current-1);
}

void DataSource::fill_combo_weapon_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current = -1)
{
    if (system->classes_list.count() < class_id) return;
    combo->clear();
    combo->addItem("(None)");
    combo->setCurrentIndex(0);


    for (int i = 0; i < system->classes_list.at(class_id-1)->weapon_set.length(); i++)
    {
        int id = system->classes_list.at(class_id-1)->weapon_set.at(i);
        QString name = system->weapons_list.at(id-1)->name;

        combo->addItem(shownum ? QString("%1: " + name).arg(id,chars,10,QChar('0')) : name);

        if (id == current)
            combo->setCurrentIndex(combo->count()-1);
    }
}

void DataSource::fill_combo_armor_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current, int amor_type)
{
    if (system->classes_list.count() < class_id) return;
    combo->clear();
    combo->addItem("(None)");
    combo->setCurrentIndex(0);


    for (int i = 0; i < system->classes_list.at(class_id-1)->armor_set.length(); i++)
    {
        int id = system->classes_list.at(class_id-1)->armor_set.at(i);
        QString name = system->armors_list.at(id-1)->name;
        if (system->armors_list.at(id-1)->kind == amor_type) //0 == Shield, 1 == Helmet, 2 == Body, 3 == Accessory
        {
            combo->addItem(shownum ? QString("%1: " + name).arg(id,chars,10,QChar('0')) : name);
            if (id == current)
                combo->setCurrentIndex(combo->count()-1);
        }
    }
}

void DataSource::fill_combo_shield_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current)
{
    this->fill_combo_armor_by_class(combo,class_id, shownum, chars, current, 0);
}

void DataSource::fill_combo_helmet_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current)
{
    this->fill_combo_armor_by_class(combo,class_id, shownum, chars, current, 1);
}

void DataSource::fill_combo_body_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current)
{
    this->fill_combo_armor_by_class(combo,class_id, shownum, chars, current, 2);
}

void DataSource::fill_combo_accessory_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current)
{
    this->fill_combo_armor_by_class(combo,class_id, shownum, chars, current, 3);
}

