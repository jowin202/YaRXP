#include "rpgeditorcontroller.h"
#include "rpgdb.h"

RPGEditorController::RPGEditorController(RPGDB *db)
    : QObject()
{
    this->db = db;
}

void RPGEditorController::connect_string_to_text_field(int object_type, QString key, QLineEdit *lineEdit)
{
    connect(lineEdit, &QLineEdit::textChanged, this, [=]() { this->obj_set_jsonvalue(object_type, key, QJsonValue(((QLineEdit*)sender())->text()) ); });
    connect(this, this->obj_changed_signals[object_type], lineEdit, [=]() { lineEdit->setText(this->obj_get_jsonvalue(object_type,key).toString()); });
}

void RPGEditorController::connect_int_to_spin_box(int object_type, QString key, QSpinBox *spinbox)
{
    connect(spinbox, &QSpinBox::valueChanged, this, [=]() { this->obj_set_jsonvalue(object_type, key, QJsonValue(((QSpinBox*)sender())->value())); });
    connect(this, this->obj_changed_signals[object_type], spinbox, [=]() { spinbox->setValue(this->obj_get_jsonvalue(object_type,key).toInt()); });
}

void RPGEditorController::connect_bool_to_check_box(int object_type, QString key, QCheckBox *checkbox)
{
    connect(checkbox, &QCheckBox::toggled, this, [=]() { this->obj_set_jsonvalue(object_type, key, QJsonValue(((QCheckBox*)sender())->isChecked())); });
    connect(this, this->obj_changed_signals[object_type], checkbox, [=]() { checkbox->setChecked(this->obj_get_jsonvalue(object_type,key).toBool()); });
}

void RPGEditorController::connect_int_to_combo_box(int object_type, QString key, QComboBox *combo)
{
    connect(combo, &QComboBox::currentIndexChanged, this, [=]() { this->obj_set_jsonvalue(object_type, key, QJsonValue(((QComboBox*)sender())->currentIndex())); });
    connect(this, this->obj_changed_signals[object_type], combo, [=]() { combo->setCurrentIndex(this->obj_get_jsonvalue(object_type,key).toInt()); });
}


void RPGEditorController::connect_int_to_data_combo_box(int object_type, QString key, QComboBox *combo, int content_type, bool shownum, int chars, bool allow_none)
{
    connect(combo, &QComboBox::currentIndexChanged, this, [=]() { this->obj_set_jsonvalue(object_type, key, QJsonValue(((QComboBox*)sender())->currentData().toInt())); });
    connect(this, this->obj_changed_signals[object_type], combo, [=]()
    {
        this->fill_combo(combo, content_type, shownum, chars, allow_none);
        for (int i = 0; i < combo->count(); i++)
            if (this->obj_get_jsonvalue(object_type,key).toInt() == combo->itemData(i).toInt())
                combo->setCurrentIndex(i);
    });
}

void RPGEditorController::connect_array_to_checkbox_list(int object_type, QString key, CheckBoxList *list, int content_type)
{
    connect(list, &CheckBoxList::values_changed, this, [=]() {
        this->obj_set_jsonvalue(object_type, key, list->get_result());
    });
    connect(this, this->obj_changed_signals[object_type], list, [=]()
    {
        list->clear();
        QStringList name_list = this->obj_get_name_list(content_type);
        QJsonArray bool_information = this->obj_get_jsonvalue(object_type, key).toArray();
        for (int i = 0; i < name_list.length(); i++)
        {
            list->add_checkbox(name_list.at(i), bool_information.contains(i+1));
        }
    });
}

void RPGEditorController::connect_table_to_abc_list(int object_type, QString key, ABCList *list, int content_type)
{
    connect(list, &ABCList::values_changed, this, [=]() {
        QJsonObject table = this->obj_get_jsonvalue(object_type, key).toObject();
        QJsonArray array = ((ABCList*)sender())->get_result();
        table.insert("values", array);
        this->obj_set_jsonvalue(object_type, key, table);

    });
    connect(this, this->obj_changed_signals[object_type], list, [=]()
    {
        list->clear();
        QStringList name_list = this->obj_get_name_list(content_type);
        QJsonObject table = this->obj_get_jsonvalue(object_type, key).toObject();
        QJsonArray array = table.value("values").toArray();

        for (int i = 0; i < name_list.length(); i++)
        {
            list->add_box(name_list.at(i), array.at(i+1).toInt());
        }
    });
}

void RPGEditorController::connect_image_display_widget(int object_type, int image_type, QString key, QString key_hue, ImageDisplayWidget *widget)
{
    connect(widget, &ImageDisplayWidget::image_changed, this, [=]() {

    });
    connect(this, this->obj_changed_signals[object_type], widget, [=]()
    {
        if (key_hue == "")
            widget->set_data(this->get_db(), image_type, this->obj_get_jsonvalue(object_type, key).toString());
        else
            widget->set_data(this->get_db(), image_type, this->obj_get_jsonvalue(object_type, key).toString(), this->obj_get_jsonvalue(object_type, key_hue).toInt());
    });

}


void RPGEditorController::set_current_object(int object_type, int object_value)
{
    *this->current_instance_variables[object_type] = object_value;
    this->refresh(object_type);
}

QJsonValue RPGEditorController::obj_get_jsonvalue(int obj_type, QString key)
{
    if (obj_type == RPGDB::ACTORS && current_actor < 0) return QJsonValue();
    else if (obj_type == RPGDB::ANIMATIONS && current_animation < 0) return QJsonValue();
    else if (obj_type == RPGDB::ARMORS && current_armor < 0) return QJsonValue();
    else if (obj_type == RPGDB::CLASSES && current_class < 0) return QJsonValue();
    else if (obj_type == RPGDB::COMMONEVENTS && current_common_event < 0) return QJsonValue();
    else if (obj_type == RPGDB::ENEMIES && current_enemy < 0) return QJsonValue();
    else if (obj_type == RPGDB::ITEMS && current_item < 0) return QJsonValue();
    else if (obj_type == RPGDB::SKILLS && current_skill < 0) return QJsonValue();
    else if (obj_type == RPGDB::STATES && current_state < 0) return QJsonValue();
    else if (obj_type == RPGDB::TILESETS && current_tileset < 0) return QJsonValue();
    else if (obj_type == RPGDB::TROOPS && current_troop < 0) return QJsonValue();
    else if (obj_type == RPGDB::WEAPONS && current_weapon < 0) return QJsonValue();

    if (obj_type == RPGDB::ACTORS)
        return actor_file.array().at(current_actor).toObject().value(key);
    else if (obj_type == RPGDB::CLASSES)
        return class_file.array().at(current_class).toObject().value(key);
    else if (obj_type == RPGDB::SKILLS)
        return skill_file.array().at(current_skill).toObject().value(key);
    else if (obj_type == RPGDB::ITEMS)
        return item_file.array().at(current_item).toObject().value(key);
    else if (obj_type == RPGDB::WEAPONS)
        return weapon_file.array().at(current_weapon).toObject().value(key);
    else if (obj_type == RPGDB::ARMORS)
        return armor_file.array().at(current_armor).toObject().value(key);
    else if (obj_type == RPGDB::ENEMIES)
        return enemy_file.array().at(current_enemy).toObject().value(key);
    else if (obj_type == RPGDB::TROOPS)
        return troop_file.array().at(current_troop).toObject().value(key);
    else if (obj_type == RPGDB::STATES)
        return state_file.array().at(current_state).toObject().value(key);
    else if (obj_type == RPGDB::ANIMATIONS)
        return animation_file.array().at(current_animation).toObject().value(key);
    else if (obj_type == RPGDB::TILESETS)
        return tileset_file.array().at(current_tileset).toObject().value(key);
    else if (obj_type == RPGDB::COMMONEVENTS)
        return common_event_file.array().at(current_common_event).toObject().value(key);

    return QJsonValue();
}

QStringList RPGEditorController::obj_get_name_list(int obj_type)
{
    QJsonDocument doc;

    if (obj_type == RPGDB::ACTORS)
        doc = QJsonDocument(actor_file);
    else if (obj_type == RPGDB::CLASSES)
        doc = QJsonDocument(class_file);
    else if (obj_type == RPGDB::SKILLS)
        doc = QJsonDocument(skill_file);
    else if (obj_type == RPGDB::ITEMS)
        doc = QJsonDocument(item_file);
    else if (obj_type == RPGDB::WEAPONS)
        doc = QJsonDocument(weapon_file);
    else if (obj_type == RPGDB::ARMORS)
        doc = QJsonDocument(armor_file);
    else if (obj_type == RPGDB::ENEMIES)
        doc = QJsonDocument(enemy_file);
    else if (obj_type == RPGDB::TROOPS)
        doc = QJsonDocument(troop_file);
    else if (obj_type == RPGDB::STATES)
        doc = QJsonDocument(state_file);
    else if (obj_type == RPGDB::ANIMATIONS)
        doc = QJsonDocument(animation_file);
    else if (obj_type == RPGDB::TILESETS)
        doc = QJsonDocument(tileset_file);
    else if (obj_type == RPGDB::COMMONEVENTS)
        doc = QJsonDocument(common_event_file);
    else if (obj_type == RPGDB::ELEMENTS)
    {
        doc = QJsonDocument(system_file);
        QJsonArray arr = doc.object().value("@elements").toArray();

        QStringList list;
        for (int i = 1; i < arr.count(); i++)
        {
            list << arr.at(i).toString();
        }
        return list;
    }

    QStringList list;
    for (int i = 1; i < doc.array().count(); i++)
    {
        list << doc.array().at(i).toObject().value("@name").toString();
    }
    return list;
}


void RPGEditorController::fill_combo(QComboBox *combo, int type, bool shownum, int chars, bool allow_none)
{
    combo->clear();

    if (allow_none)
        combo->addItem("(None)",QVariant(0));

    QJsonDocument doc;

    if (type < RPGDB::CLASSES_WEAPONS)
    {
        QStringList obj_name_list = this->obj_get_name_list(type);

        for (int i = 0; i < obj_name_list.size(); i++)
        {
            combo->addItem(shownum ? QString("%1: " + obj_name_list.at(i)).arg(i+1,chars,10,QChar('0')) :
                                     doc.array().at(i).toObject().value("@name").toString(), QVariant(i+1));
        }
    }
    else if (type == RPGDB::CLASSES_WEAPONS) //for filtered lists at actors page
    {
        doc = QJsonDocument(weapon_file);
        bool ok = true;
        int class_id = this->obj_get_jsonvalue(RPGDB::ACTORS, "@class_id").toVariant().toInt(&ok); // get current actor class_id
        if (!ok) return;

        QJsonObject dest_class = class_file.array().at(class_id).toObject(); // do not need + 1 here
        QJsonArray weapon_set = dest_class.value("@weapon_set").toArray();

        for (int i = 0; i < weapon_set.size(); i++)
        {
            int key = weapon_set.at(i).toInt();
            combo->addItem(shownum ? QString("%1: " + doc.array().at(key).toObject().value("@name").toString()).arg(key,chars,10,QChar('0')) :
                                     doc.array().at(key).toObject().value("@name").toString(), QVariant(key));
        }
    }
    else //for filtered lists at actors page
    {
        doc = QJsonDocument(armor_file);
        bool ok = true;
        int class_id = this->obj_get_jsonvalue(RPGDB::ACTORS, "@class_id").toVariant().toInt(&ok); // get current actor class_id
        if (!ok) return;

        QJsonObject dest_class = class_file.array().at(class_id).toObject(); // do not need + 1 here
        QJsonArray armor_set = dest_class.value("@armor_set").toArray();

        for (int i = 0; i < armor_set.size(); i++)
        {
            int key = armor_set.at(i).toInt();
            //0 == Shield, 1 == Helmet, 2 == Body, 3 == Accessory
            if (type == RPGDB::CLASSES_SHIELD && doc.array().at(key).toObject().value("@kind").toInt() != 0)
                continue;
            else if (type == RPGDB::CLASSES_HELMET && doc.array().at(key).toObject().value("@kind").toInt() != 1)
                continue;
            else if (type == RPGDB::CLASSES_BODY && doc.array().at(key).toObject().value("@kind").toInt() != 2)
                continue;
            else if (type == RPGDB::CLASSES_ACCESSORY && doc.array().at(key).toObject().value("@kind").toInt() != 3)
                continue;

            combo->addItem(shownum ? QString("%1: " + doc.array().at(key).toObject().value("@name").toString()).arg(key,chars,10,QChar('0')) :
                                     doc.array().at(key).toObject().value("@name").toString(), QVariant(key));
        }
    }
}

void RPGEditorController::set_files(QJsonDocument actor_file, QJsonDocument animation_file, QJsonDocument armor_file, QJsonDocument class_file, QJsonDocument common_event_file, QJsonDocument enemy_file, QJsonDocument item_file, QJsonDocument skill_file, QJsonDocument state_file, QJsonDocument system_file, QJsonDocument tileset_file, QJsonDocument troop_file, QJsonDocument weapon_file)
{
    this->actor_file = actor_file;
    this->animation_file = animation_file;
    this->armor_file = armor_file;
    this->class_file = class_file;
    this->common_event_file = common_event_file;
    this->enemy_file = enemy_file;
    this->item_file = item_file;
    this->skill_file = skill_file;
    this->state_file = state_file;
    this->system_file = system_file;
    this->tileset_file = tileset_file;
    this->troop_file = troop_file;
    this->weapon_file = weapon_file;
}

void RPGEditorController::obj_set_jsonvalue(int obj_type, QString key, QJsonValue value)
{
    if (block_writing)
        return;

    QJsonDocument *doc;
    int current = -1;

    if (obj_type == RPGDB::ACTORS && current_actor > 0 && current_actor < actor_file.array().count())
    {
        doc = &actor_file;
        current = this->current_actor;
    }
    else if (obj_type == RPGDB::ANIMATIONS && current_animation > 0 && current_animation < animation_file.array().count())
    {
        doc = &animation_file;
        current = this->current_animation;
    }
    else if (obj_type == RPGDB::ARMORS && current_armor > 0 && current_armor < armor_file.array().count())
    {
        doc = &armor_file;
        current = this->current_armor;
    }
    else if (obj_type == RPGDB::CLASSES && current_class > 0 && current_class < class_file.array().count())
    {
        doc = &class_file;
        current = this->current_class;
    }
    else if (obj_type == RPGDB::COMMONEVENTS && current_common_event > 0 && current_common_event < common_event_file.array().count())
    {
        doc = &common_event_file;
        current = this->current_common_event;
    }
    else if (obj_type == RPGDB::ENEMIES && current_enemy > 0 && current_enemy < enemy_file.array().count())
    {
        doc = &enemy_file;
        current = this->current_enemy;
    }
    else if (obj_type == RPGDB::ITEMS && current_item > 0 && current_item < item_file.array().count())
    {
        doc = &item_file;
        current = this->current_item;
    }
    else if (obj_type == RPGDB::SKILLS && current_skill > 0 && current_skill < skill_file.array().count())
    {
        doc = &skill_file;
        current = this->current_skill;
    }
    else if (obj_type == RPGDB::STATES && current_state > 0 && current_state < state_file.array().count())
    {
        doc = &state_file;
        current = this->current_state;
    }
    else if (obj_type == RPGDB::TILESETS && current_tileset > 0 && current_tileset < tileset_file.array().count())
    {
        doc = &tileset_file;
        current = this->current_tileset;
    }
    else if (obj_type == RPGDB::TROOPS && current_troop > 0 && current_troop < troop_file.array().count())
    {
        doc = &troop_file;
        current = this->current_troop;
    }
    else if (obj_type == RPGDB::WEAPONS && current_weapon > 0 && current_weapon < weapon_file.array().count())
    {
        doc = &weapon_file;
        current = this->current_weapon;
    }
        else return;


    QJsonArray arr = doc->array();
    QJsonObject obj = arr.at(current).toObject();
    obj[key] = value;
    arr.removeAt(current);
    arr.insert(current,obj);

    doc->setArray(arr);

}

void RPGEditorController::current_actor_set_parameters(QJsonArray params)
{
    if (current_actor < 0) return;
    QJsonArray arr = actor_file.array();
    QJsonObject obj = arr.at(current_actor).toObject();
    QJsonObject obj_table = obj.value("@parameters").toObject();

    obj_table.insert("values", params);
    obj.insert("@parameters", obj_table);
    arr.removeAt(current_actor);
    arr.insert(current_actor, obj);
    actor_file.setArray(arr);

    QJsonArray obj_table_values = obj_table.value("values").toArray();
}

void RPGEditorController::refresh(int object_type)
{

    if (object_type == RPGDB::ACTORS)
    {
        this->block_writing = true;
        emit current_actor_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::ANIMATIONS)
    {
        this->block_writing = true;
        emit current_animation_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::ARMORS)
    {
        this->block_writing = true;
        emit current_armor_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::CLASSES)
    {
        this->block_writing = true;
        emit current_class_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::COMMONEVENTS)
    {
        this->block_writing = true;
        emit current_common_event_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::ENEMIES)
    {
        this->block_writing = true;
        emit current_enemy_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::ITEMS)
    {
        this->block_writing = true;
        emit current_item_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::SKILLS)
    {
        this->block_writing = true;
        emit current_skill_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::STATES)
    {
        this->block_writing = true;
        emit current_state_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::TILESETS)
    {
        this->block_writing = true;
        emit current_tileset_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::TROOPS)
    {
        this->block_writing = true;
        emit current_troop_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::WEAPONS)
    {
        this->block_writing = true;
        emit current_weapon_changed();
        this->block_writing = false;
    }
}

