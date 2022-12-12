#include "editenemies.h"
#include "ui_editenemies.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgeditorcontroller.h"

#include "dialogs/imagedialog.h"
#include "enemyactiondialog.h"

EditEnemies::EditEnemies(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditEnemies)
{
    ui->setupUi(this);

    this->ui->table_action->verticalHeader()->setVisible(false);

    action_add = new QAction("Add");
    action_add->setShortcut(Qt::Key_Return);
    connect(action_add, SIGNAL(triggered()), this, SLOT(item_add()));
    this->ui->table_action->addAction(action_add);

    action_edit = new QAction("Edit");
    action_edit->setShortcut(Qt::Key_Space);
    connect(action_edit, SIGNAL(triggered()), this, SLOT(item_edit()));
    this->ui->table_action->addAction(action_edit);

    action_delete = new QAction("Delete");
    action_delete->setShortcut(Qt::Key_Delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(item_delete()));
    this->ui->table_action->addAction(action_delete);

    action_copy = new QAction("Copy");
    action_copy->setShortcut(QKeySequence(tr("Ctrl+C")));
    connect(action_copy, SIGNAL(triggered()), this, SLOT(item_copy()));
    this->ui->table_action->addAction(action_copy);


    action_paste = new QAction("Paste");
    action_paste->setShortcut(QKeySequence(tr("Ctrl+V")));
    connect(action_paste, SIGNAL(triggered()), this, SLOT(item_paste()));
    this->ui->table_action->addAction(action_paste);

}

EditEnemies::~EditEnemies()
{
    delete ui;
}

void EditEnemies::setEC(RPGEditorController *ec)
{
    this->ec = ec;

    this->ec->connect_string_to_text_field(RPGDB::ENEMIES, "@name", this->ui->line_name);

    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@maxhp", this->ui->spin_maxhp);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@maxsp", this->ui->spin_maxsp);

    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@str", this->ui->spin_str);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@dex", this->ui->spin_dex);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@agi", this->ui->spin_agi);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@int", this->ui->spin_int);

    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@atk", this->ui->spin_atk);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@pdef", this->ui->spin_pdef);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@mdef", this->ui->spin_mdef);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@eva", this->ui->spin_eva);

    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@animation1_id", this->ui->combo_attacker, RPGDB::ANIMATIONS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@animation2_id", this->ui->combo_target, RPGDB::ANIMATIONS, true, 3, true);

    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@exp", this->ui->spin_exp);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@gold", this->ui->spin_gold);

    this->ec->connect_table_to_abc_list(RPGDB::ENEMIES, "@element_ranks", this->ui->element_widget, RPGDB::ELEMENTS);
    this->ec->connect_table_to_abc_list(RPGDB::ENEMIES, "@state_ranks", this->ui->state_widget, RPGDB::STATES);

    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@armor_id", this->ui->combo_armor, RPGDB::ARMORS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@item_id", this->ui->combo_item, RPGDB::ITEMS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@weapon_id", this->ui->combo_weapon, RPGDB::WEAPONS, true, 3, true);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@treasure_prob", this->ui->spin_prob);

    this->ec->connect_image_display_widget(RPGDB::ENEMIES, ImageDialog::BATTLERS, "@battler_name", "@battler_hue", this->ui->label_battler_img);


    connect(this->ec, SIGNAL(current_enemy_changed()), this, SLOT(update_actions_from_file()));

}

void EditEnemies::update_actions_from_file()
{
    this->ui->table_action->clearContents();
    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();
    QStringList skill_names = this->ec->obj_get_name_list(RPGDB::SKILLS);
    int rowCount = actions.count();
    this->ui->table_action->setRowCount(rowCount);


    for (int row = 0; row < rowCount; row++)
    {
        int condition_turn_a = actions.at(row).toObject().value("@condition_turn_a").toInt();
        int condition_turn_b = actions.at(row).toObject().value("@condition_turn_b").toInt();
        int condition_hp = actions.at(row).toObject().value("@condition_hp").toInt();
        int condition_level = actions.at(row).toObject().value("@condition_level").toInt();
        int condition_switch_id = actions.at(row).toObject().value("@condition_switch_id").toInt();

        int kind = actions.at(row).toObject().value("@kind").toInt();
        //int rating = actions.at(row).toObject().value("@rating").toInt();
        int skill_id = actions.at(row).toObject().value("@skill_id").toInt();
        int basic = actions.at(row).toObject().value("@basic").toInt();


        QString condition_turns = "";
        if (condition_turn_a == 0 && condition_turn_b == 1) condition_turns = "";
        else if (condition_turn_a == 0 && condition_turn_b == 0) condition_turns = "Turn 0";
        else if (condition_turn_a == 0 && condition_turn_b > 1) condition_turns = QString("Turn %1X").arg(condition_turn_b);
        else if (condition_turn_a > 0 && condition_turn_b == 0) condition_turns = QString("Turn %1").arg(condition_turn_a);
        else condition_turns = QString("Turn %1+%2X").arg(condition_turn_a).arg(condition_turn_b);


        if (kind == 0) //Basic
            this->ui->table_action->setItem(row,0,new QTableWidgetItem(this->basic_actions.at(basic)));
        else if (kind == 1) //Skill
        {
            QStringList skill_names = ec->obj_get_name_list(RPGDB::SKILLS);
            if (skill_id <= skill_names.length())
                this->ui->table_action->setItem(row,0,new QTableWidgetItem(skill_names.at(skill_id-1)));
        }
        else this->ui->table_action->setItem(row,0,new QTableWidgetItem());
        this->set_readonly(this->ui->table_action->item(row,0));

        this->ui->table_action->setItem(row,1,new QTableWidgetItem(condition_turns));
        this->set_readonly(this->ui->table_action->item(row,0));

        this->ui->table_action->setItem(row,2,new QTableWidgetItem(condition_hp < 100 ? QString("%1\% HP or below").arg(condition_hp) : ""));
        this->set_readonly(this->ui->table_action->item(row,2));

        this->ui->table_action->setItem(row,3,new QTableWidgetItem(condition_level > 1 ? QString("Level %1 or above").arg(condition_level) : ""));
        this->set_readonly(this->ui->table_action->item(row,3));

        this->ui->table_action->setItem(row,4,new QTableWidgetItem(condition_switch_id > 0 ? QString("Switch [%1] is ON").arg(condition_switch_id,4,10,QChar('0')) : ""));
        this->set_readonly(this->ui->table_action->item(row,4));

    }

    this->ui->table_action->resizeColumnsToContents();
}


void EditEnemies::change_action(int row, int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int skill, int rating, int basic)
{
    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();
    QJsonObject new_action = Factory().create_enemy_action(turn_a, turn_b,hp,level,switch_id,kind,skill,rating,basic);
    actions.removeAt(row);
    actions.insert(row,new_action);
    this->ec->obj_set_jsonvalue(RPGDB::ENEMIES, "@actions", actions);

    this->update_actions_from_file();
    this->ui->table_action->selectRow(row); //TODO Check this
}

void EditEnemies::on_table_action_itemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    this->item_edit();
}

void EditEnemies::on_button_action_del_clicked()
{
    this->item_delete();
}

void EditEnemies::on_button_action_add_clicked()
{
    this->item_add();
}

void EditEnemies::item_add()
{
    EnemyActionDialog *dialog = new EnemyActionDialog(ec, this->ui->table_action->rowCount(),0,1,100, 1,0,0,1,5,0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int,int,int,int,int,int,int,int)), this, SLOT(change_action(int,int,int,int,int,int,int,int,int,int)));
    dialog->show();
}

void EditEnemies::item_edit()
{
    int row = this->ui->table_action->currentRow();
    if (row < 0) return;

    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();

    int condition_turn_a = actions.at(row).toObject().value("@condition_turn_a").toInt();
    int condition_turn_b = actions.at(row).toObject().value("@condition_turn_b").toInt();
    int condition_hp = actions.at(row).toObject().value("@condition_hp").toInt();
    int condition_level = actions.at(row).toObject().value("@condition_level").toInt();
    int condition_switch_id = actions.at(row).toObject().value("@condition_switch_id").toInt();
    int kind = actions.at(row).toObject().value("@kind").toInt();
    int rating = actions.at(row).toObject().value("@rating").toInt();
    int skill_id = actions.at(row).toObject().value("@skill_id").toInt();
    int basic = actions.at(row).toObject().value("@basic").toInt();


    EnemyActionDialog *dialog = new EnemyActionDialog(ec, row, condition_turn_a, condition_turn_b, condition_hp, condition_level, condition_switch_id
                                                      ,kind,rating,skill_id,basic);
    connect(dialog, SIGNAL(ok_clicked(int,int,int,int,int,int,int,int,int,int)), this, SLOT(change_action(int,int,int,int,int,int,int,int,int,int)));
    dialog->show();
}

void EditEnemies::item_delete()
{
    int row = this->ui->table_action->currentRow();
    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();
    actions.removeAt(row);
    this->ec->obj_set_jsonvalue(RPGDB::ENEMIES, "@actions", actions);

    this->update_actions_from_file();
}

void EditEnemies::item_copy()
{
    int row = this->ui->table_action->currentRow();
    QJsonObject action = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray().at(row).toObject();
    QJsonDocument doc(action);

    QSettings settings;
    settings.setValue("copied_item_enemy_action", doc.toJson(QJsonDocument::Compact));
}

void EditEnemies::item_paste()
{
    int row = this->ui->table_action->currentRow();
    if (row < 0) row = 0;


    QSettings settings;
    QByteArray json = settings.value("copied_item_enemy_action").toByteArray();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
    if (err.error != QJsonParseError::NoError) return;

    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();
    actions.insert(row,doc.object());
    this->ec->obj_set_jsonvalue(RPGDB::ENEMIES, "@actions", actions);

    this->update_actions_from_file();
    this->ui->table_action->selectRow(row); //TODO Check this
}


void EditEnemies::on_table_action_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *item = this->ui->table_action->itemAt(pos);
    if (item) {
        QMenu menu;
        menu.addAction(action_add);
        menu.addAction(action_edit);
        menu.addAction(action_delete);
        menu.addSeparator();
        menu.addAction(action_copy);
        menu.addAction(action_paste);

        menu.exec(this->ui->table_action->mapToGlobal(pos));
    }
}

