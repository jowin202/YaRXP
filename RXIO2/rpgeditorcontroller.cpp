#include "rpgeditorcontroller.h"
#include "rpgdb.h"

RPGEditorController::RPGEditorController(RPGDB *db)
    : QObject()
{
    this->db = db;
}

void RPGEditorController::connect_string_to_text_field(int object_type, QString key, QLineEdit *lineEdit)
{
    QObject::connect(lineEdit, &QLineEdit::textChanged, this, [=]() { this->obj_set_value(object_type, key, QVariant(((QLineEdit*)sender())->text()) ); });
    if (object_type == RPGDB::ACTORS)
        connect(this, &RPGEditorController::current_actor_changed, lineEdit, [=]() { lineEdit->setText(this->obj_get_value(object_type,key).toString()); });
    else if (object_type == RPGDB::ARMORS)
        connect(this, &RPGEditorController::current_armor_changed, lineEdit, [=]() { lineEdit->setText(this->obj_get_value(object_type,key).toString()); });
}

void RPGEditorController::connect_int_to_spin_box(int object_type, QString key, QSpinBox *spinbox)
{
    QObject::connect(spinbox, &QSpinBox::valueChanged, this, [=]() { this->obj_set_value(object_type, key, QVariant(((QSpinBox*)sender())->value())); });
    if (object_type == RPGDB::ACTORS)
        connect(this, &RPGEditorController::current_actor_changed, spinbox, [=]() { spinbox->setValue(this->obj_get_value(object_type,key).toInt()); });
    else if (object_type == RPGDB::ARMORS)
        connect(this, &RPGEditorController::current_armor_changed, spinbox, [=]() { spinbox->setValue(this->obj_get_value(object_type,key).toInt()); });
}

void RPGEditorController::connect_bool_to_check_box(int object_type, QString key, QCheckBox *checkbox)
{
    QObject::connect(checkbox, &QCheckBox::toggled, this, [=]() { this->obj_set_value(object_type, key, QVariant(((QCheckBox*)sender())->isChecked())); });
    if (object_type == RPGDB::ACTORS)
        connect(this, &RPGEditorController::current_actor_changed, checkbox, [=]() { checkbox->setChecked(this->obj_get_value(object_type,key).toBool()); });
    else if (object_type == RPGDB::ARMORS)
        connect(this, &RPGEditorController::current_armor_changed, checkbox, [=]() { checkbox->setChecked(this->obj_get_value(object_type,key).toBool()); });

}

void RPGEditorController::connect_int_to_combo_box(int object_type, QString key, QComboBox *combo)
{
    QObject::connect(combo, &QComboBox::currentIndexChanged, this, [=]() { this->obj_set_value(object_type, key, QVariant(((QComboBox*)sender())->currentIndex())); });
    if (object_type == RPGDB::ACTORS)
        connect(this, &RPGEditorController::current_actor_changed, combo, [=]() { combo->setCurrentIndex(this->obj_get_value(object_type,key).toInt()); });
    else if (object_type == RPGDB::ARMORS)
        connect(this, &RPGEditorController::current_armor_changed, combo, [=]() { combo->setCurrentIndex(this->obj_get_value(object_type,key).toInt()); });
}


void RPGEditorController::connect_int_to_data_combo_box(int object_type, QString key, QComboBox *combo, int content_type, bool shownum, int chars, bool allow_none)
{
    QObject::connect(combo, &QComboBox::currentIndexChanged, this, [=]() { this->obj_set_value(object_type, key, QVariant(((QComboBox*)sender())->currentData().toInt())); });
    if (object_type == RPGDB::ACTORS)
        connect(this, &RPGEditorController::current_actor_changed, combo, [=]()
        {
            this->fill_combo(combo, content_type, shownum, chars, allow_none);
            for (int i = 0; i < combo->count(); i++)
                if (this->obj_get_value(object_type,key).toInt() == combo->itemData(i).toInt())
                    combo->setCurrentIndex(i);
        });
    else if (object_type == RPGDB::ARMORS)
        connect(this, &RPGEditorController::current_armor_changed, combo, [=]()
        {
            this->fill_combo(combo, content_type, shownum, chars, allow_none);
            for (int i = 0; i < combo->count(); i++)
                if (this->obj_get_value(object_type,key).toInt() == combo->itemData(i).toInt())
                    combo->setCurrentIndex(i);
        });

}


void RPGEditorController::set_current_object(int object_type, int object_value)
{
    if (object_type == RPGDB::ACTORS)
    {
        this->current_actor = object_value;
        this->block_writing = true;
        emit current_actor_changed();
        emit current_actor_exp_curve(actor_file.array().at(current_actor).toObject().value("@exp_basis").toInt(),
                                     actor_file.array().at(current_actor).toObject().value("@exp_inflation").toInt());

        emit current_actor_parameters(actor_file.array().at(current_actor).toObject().value("@parameters").toObject());
        this->block_writing = false;
    }
    else if (object_type == RPGDB::ANIMATIONS)
    {
        this->current_animation = object_value;
        this->block_writing = true;
        emit current_animation_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::ARMORS)
    {
        this->current_armor = object_value;
        this->block_writing = true;
        emit current_armor_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::CLASSES)
    {
        this->current_class = object_value;
        this->block_writing = true;
        emit current_class_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::COMMONEVENTS)
    {
        this->current_common_event = object_value;
        this->block_writing = true;
        emit current_common_event_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::ENEMIES)
    {
        this->current_enemy = object_value;
        this->block_writing = true;
        emit current_enemy_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::ITEMS)
    {
        this->current_item = object_value;
        this->block_writing = true;
        emit current_item_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::SKILLS)
    {
        this->current_skill = object_value;
        this->block_writing = true;
        emit current_skill_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::STATES)
    {
        this->current_state = object_value;
        this->block_writing = true;
        emit current_state_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::TILESETS)
    {
        this->current_tileset = object_value;
        this->block_writing = true;
        emit current_tileset_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::TROOPS)
    {
        this->current_troop = object_value;
        this->block_writing = true;
        emit current_troop_changed();
        this->block_writing = false;
    }
    else if (object_type == RPGDB::WEAPONS)
    {
        this->current_weapon = object_value;
        this->block_writing = true;
        emit current_weapon_changed();
        this->block_writing = false;
    }
}

QVariant RPGEditorController::obj_get_value(int obj_type, QString key)
{
    if (obj_type == RPGDB::ACTORS && current_actor < 0) return QVariant();
    else if (obj_type == RPGDB::ANIMATIONS && current_animation < 0) return QVariant();
    else if (obj_type == RPGDB::ARMORS && current_armor < 0) return QVariant();
    else if (obj_type == RPGDB::CLASSES && current_class < 0) return QVariant();
    else if (obj_type == RPGDB::COMMONEVENTS && current_common_event < 0) return QVariant();
    else if (obj_type == RPGDB::ENEMIES && current_enemy < 0) return QVariant();
    else if (obj_type == RPGDB::ITEMS && current_item < 0) return QVariant();
    else if (obj_type == RPGDB::SKILLS && current_skill < 0) return QVariant();
    else if (obj_type == RPGDB::STATES && current_state < 0) return QVariant();
    else if (obj_type == RPGDB::TILESETS && current_tileset < 0) return QVariant();
    else if (obj_type == RPGDB::TROOPS && current_troop < 0) return QVariant();
    else if (obj_type == RPGDB::WEAPONS && current_weapon < 0) return QVariant();

    if (obj_type == RPGDB::ACTORS)
        return actor_file.array().at(current_actor).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::CLASSES)
        return class_file.array().at(current_class).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::SKILLS)
        return skill_file.array().at(current_skill).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::ITEMS)
        return item_file.array().at(current_item).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::WEAPONS)
        return weapon_file.array().at(current_weapon).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::ARMORS)
        return armor_file.array().at(current_armor).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::ENEMIES)
        return enemy_file.array().at(current_enemy).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::TROOPS)
        return troop_file.array().at(current_troop).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::STATES)
        return state_file.array().at(current_state).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::ANIMATIONS)
        return animation_file.array().at(current_animation).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::TILESETS)
        return tileset_file.array().at(current_tileset).toObject().value(key).toVariant();
    else if (obj_type == RPGDB::COMMONEVENTS)
        return common_event_file.array().at(current_common_event).toObject().value(key).toVariant();

    return QVariant();
}

void RPGEditorController::fill_list(QListWidget *list, int type, bool shownum, int chars)
{
    list->clear();

    if (type == RPGDB::ACTORS)
        for (int i = 1; i < actor_file.array().size(); i++)
        {
            QString name = actor_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::CLASSES)
        for (int i = 1; i < class_file.array().size(); i++)
        {
            QString name = class_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::SKILLS)
        for (int i = 1; i < skill_file.array().size(); i++)
        {
            QString name = skill_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::ITEMS)
        for (int i = 1; i < item_file.array().size(); i++)
        {
            QString name = item_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::WEAPONS)
        for (int i = 1; i < weapon_file.array().size(); i++)
        {
            QString name = weapon_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::ARMORS)
        for (int i = 1; i < armor_file.array().size(); i++)
        {
            QString name = armor_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::ENEMIES)
        for (int i = 1; i < enemy_file.array().size(); i++)
        {
            QString name = enemy_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::TROOPS)
        for (int i = 1; i < troop_file.array().size(); i++)
        {
            QString name = troop_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::STATES)
        for (int i = 1; i < state_file.array().size(); i++)
        {
            QString name = state_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::ANIMATIONS)
        for (int i = 1; i < animation_file.array().size(); i++)
        {
            QString name = animation_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::TILESETS)
        for (int i = 1; i < tileset_file.array().size(); i++)
        {
            QString name = tileset_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
    else if (type == RPGDB::COMMONEVENTS)
        for (int i = 1; i < common_event_file.array().size(); i++)
        {
            QString name = common_event_file.array().at(i).toObject().value("@name").toString();
            list->addItem(shownum ? QString("%1: " + name).arg(i+1,chars,10,QChar('0')) : name);
        }
}

void RPGEditorController::fill_combo(QComboBox *combo, int type, bool shownum, int chars, bool allow_none)
{
    combo->clear();

    if (allow_none)
        combo->addItem("(None)",QVariant(0));


    QJsonDocument doc;

    if (type == RPGDB::ACTORS)
        doc = QJsonDocument(actor_file);
    else if (type == RPGDB::CLASSES)
        doc = QJsonDocument(class_file);
    else if (type == RPGDB::SKILLS)
        doc = QJsonDocument(skill_file);
    else if (type == RPGDB::ITEMS)
        doc = QJsonDocument(item_file);
    else if (type == RPGDB::WEAPONS)
        doc = QJsonDocument(weapon_file);
    else if (type == RPGDB::ARMORS)
        doc = QJsonDocument(armor_file);
    else if (type == RPGDB::ENEMIES)
        doc = QJsonDocument(enemy_file);
    else if (type == RPGDB::TROOPS)
        doc = QJsonDocument(troop_file);
    else if (type == RPGDB::STATES)
        doc = QJsonDocument(state_file);
    else if (type == RPGDB::ANIMATIONS)
        doc = QJsonDocument(animation_file);
    else if (type == RPGDB::TILESETS)
        doc = QJsonDocument(tileset_file);
    else if (type == RPGDB::COMMONEVENTS)
        doc = QJsonDocument(common_event_file);

    if (type < RPGDB::CLASSES_WEAPONS)
    {
        for (int i = 1; i < doc.array().size(); i++)
        {
            combo->addItem(shownum ? QString("%1: " + doc.array().at(i).toObject().value("@name").toString()).arg(i,chars,10,QChar('0')) :
                                     doc.array().at(i).toObject().value("@name").toString(), QVariant(i));
        }
    }
    else if (type == RPGDB::CLASSES_WEAPONS) //for filtered lists at actors page
    {
        doc = QJsonDocument(weapon_file);
        bool ok = true;
        int class_id = this->obj_get_value(RPGDB::ACTORS, "@class_id").toInt(&ok); // get current actor class_id
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
        int class_id = this->obj_get_value(RPGDB::ACTORS, "@class_id").toInt(&ok); // get current actor class_id
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

void RPGEditorController::obj_set_value(int obj_type, QString key, QVariant value)
{
    if (block_writing)
        return;

    if (obj_type == RPGDB::ACTORS && current_actor < 0) return;
    else if (obj_type == RPGDB::ANIMATIONS && current_animation < 0) return;
    else if (obj_type == RPGDB::ARMORS && current_armor < 0) return;
    else if (obj_type == RPGDB::CLASSES && current_class < 0) return;
    else if (obj_type == RPGDB::COMMONEVENTS && current_common_event < 0) return;
    else if (obj_type == RPGDB::ENEMIES && current_enemy < 0) return;
    else if (obj_type == RPGDB::ITEMS && current_item < 0) return;
    else if (obj_type == RPGDB::SKILLS && current_skill < 0) return;
    else if (obj_type == RPGDB::STATES && current_state < 0) return;
    else if (obj_type == RPGDB::TILESETS && current_tileset < 0) return;
    else if (obj_type == RPGDB::TROOPS && current_troop < 0) return;
    else if (obj_type == RPGDB::WEAPONS && current_weapon < 0) return;



    QJsonDocument *doc = 0;
    int current = 0;

    if (obj_type == RPGDB::ACTORS && current_actor < actor_file.array().size())
    {
        doc = &this->actor_file;
        current = this->current_actor;
    }
    else if (obj_type == RPGDB::ARMORS && current_actor < armor_file.array().size())
    {
        doc = &this->armor_file;
        current = this->current_armor;
    }

    if (doc == 0) return;

    QJsonArray arr = doc->array();
    QJsonObject obj = arr.at(current).toObject();
    if (value.typeId() == QMetaType::Int)
        obj[key] = value.toInt();
    else if (value.typeId() == QMetaType::Bool)
        obj[key] = value.toBool();
    else if (value.typeId() == QMetaType::QString)
        obj[key] = value.toString();
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

