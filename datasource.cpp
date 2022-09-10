
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


void DataSource::fill_combo(QComboBox *combo, int type, bool shownum, int chars, int current = -1, bool allow_none = false)
{
    combo->clear();

    if (allow_none)
        combo->addItem("(None)",QVariant(0));

    if (type == ACTORS)
        for (int i = 0; i < system->actor_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->actor_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->actor_list.at(i)->name, QVariant(i+1));
    else if (type == CLASSES)
        for (int i = 0; i < system->classes_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->classes_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->classes_list.at(i)->name, QVariant(i+1));
    else if (type == SKILLS)
        for (int i = 0; i < system->skills_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->skills_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->skills_list.at(i)->name, QVariant(i+1));
    else if (type == ITEMS)
        for (int i = 0; i < system->items_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->items_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->items_list.at(i)->name, QVariant(i+1));
    else if (type == WEAPONS)
        for (int i = 0; i < system->weapons_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->weapons_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->weapons_list.at(i)->name, QVariant(i+1));
    else if (type == ARMORS)
        for (int i = 0; i < system->armors_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->armors_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->armors_list.at(i)->name, QVariant(i+1));
    else if (type == ENEMIES)
        for (int i = 0; i < system->enemies_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->enemies_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->enemies_list.at(i)->name, QVariant(i+1));
    else if (type == TROOPS)
        for (int i = 0; i < system->troops_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->troops_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->troops_list.at(i)->name, QVariant(i+1));
    else if (type == STATES)
        for (int i = 0; i < system->states_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->states_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->states_list.at(i)->name, QVariant(i+1));
    else if (type == ANIMATIONS)
        for (int i = 0; i < system->animation_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->animation_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->animation_list.at(i)->name, QVariant(i+1));
    else if (type == TILESETS)
        for (int i = 0; i < system->tileset_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->tileset_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->tileset_list.at(i)->name, QVariant(i+1));
    else if (type == COMMONEVENTS)
        for (int i = 0; i < system->common_events_list.length(); i++)
            combo->addItem(shownum ? QString("%1: " + system->common_events_list.at(i)->name).arg(i+1,chars,10,QChar('0')) :
                                    system->common_events_list.at(i)->name, QVariant(i+1));

    if (current >= 0 && combo->count() >= current)
        combo->setCurrentIndex(current-1 + (allow_none ? 1 : 0)); // +1 when (None) activated
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

QString DataSource::get_switch_name(int from_id, int to_id, bool shownum, int chars, bool brackets)
{
    QString sw;

    if (from_id == to_id)
    {
        if (from_id >= 0 && from_id <= this->system->switches_names.length() )
            sw = this->system->switches_names.at(from_id-1);
        else if (brackets && chars)
            return QString("[%1]").arg(from_id, chars, 10, QChar('0'));
        else if (brackets)
            return QString("[%1]").arg(from_id);
        else
            return QString("%1").arg(from_id);

        if (shownum)
            sw = QString("%1: %2").arg(from_id, chars, 10, QChar('0')).arg(sw);

        if (brackets)
            sw = "[" + sw + "]";
    }
    else
    {
        //not equal

        if (from_id > to_id)
        { //swap
            int tmp = to_id; to_id = from_id; from_id = tmp;
        }
        sw = QString("[%1..%2]").arg(from_id,chars,10,QChar('0')).arg(to_id,chars,10,QChar('0'));
    }


    return sw;
}

QString DataSource::get_variable_name(int from_id, int to_id, bool shownum, int chars, bool brackets)
{
    QString var;

    if (from_id == to_id)
    {
        if (from_id >= 0 && from_id <= this->system->variable_names.length() )
            var = this->system->variable_names.at(from_id-1);
        else if (brackets && chars)
            return QString("[%1]").arg(from_id, chars, 10, QChar('0'));
        else if (brackets)
            return QString("[%1]").arg(from_id);
        else
            return QString("%1").arg(from_id);

        if (shownum)
            var = QString("%1: %2").arg(from_id, chars, 10, QChar('0')).arg(var);

        if (brackets)
            var = "[" + var + "]";
    }
    else
    {
        //not equal

        if (from_id > to_id)
        { //swap
            int tmp = to_id; to_id = from_id; from_id = tmp;
        }


        var = QString("[%1..%2]").arg(from_id,chars,10,QChar('0')).arg(to_id,chars,10,QChar('0'));

    }
    return var;
}

QString DataSource::get_obj_name_by_id(int id, int type, bool shownum, int chars, bool brackets)
{
    QString result;
    if (type == ACTORS && system->actor_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->actor_list.at(id-1)->name);
        else
            result = system->actor_list.at(id-1)->name;
    }
    else if (type == CLASSES && system->classes_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->classes_list.at(id-1)->name);
        else
            result = system->classes_list.at(id-1)->name;
    }
    else if (type == SKILLS && system->skills_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->skills_list.at(id-1)->name);
        else
            result = system->skills_list.at(id-1)->name;
    }
    else if (type == ITEMS && system->items_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->items_list.at(id-1)->name);
        else
            result = system->items_list.at(id-1)->name;
    }
    else if (type == WEAPONS && system->weapons_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->weapons_list.at(id-1)->name);
        else
            result = system->weapons_list.at(id-1)->name;
    }
    else if (type == ARMORS && system->armors_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->armors_list.at(id-1)->name);
        else
            result = system->armors_list.at(id-1)->name;
    }
    else if (type == ENEMIES && system->enemies_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->enemies_list.at(id-1)->name);
        else
            result = system->enemies_list.at(id-1)->name;
    }
    else if (type == TROOPS && system->troops_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->troops_list.at(id-1)->name);
        else
            result = system->troops_list.at(id-1)->name;
    }
    else if (type == STATES && system->states_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->states_list.at(id-1)->name);
        else
            result = system->states_list.at(id-1)->name;
    }
    else if (type == ANIMATIONS && system->animation_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->animation_list.at(id-1)->name);
        else
            result = system->animation_list.at(id-1)->name;
    }
    else if (type == COMMONEVENTS && system->common_events_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->common_events_list.at(id-1)->name);
        else
            result = system->common_events_list.at(id-1)->name;
    }
    else if (type == TILESETS && system->tileset_list.length() >= id)
    {
        if (shownum)
            result = QString("%1: %2").arg(id,chars,10,QChar('0')).arg(system->tileset_list.at(id-1)->name);
        else
            result = system->tileset_list.at(id-1)->name;
    }

    if (brackets)
        result = "[" + result + "]";

    return result;
}

