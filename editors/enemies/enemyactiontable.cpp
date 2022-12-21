#include "enemyactiontable.h"
#include "enemyactiondialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "RXIO2/factory.h"

EnemyActionTable::EnemyActionTable(QWidget *parent) : QTableWidget(parent)
{

    action_add = new QAction("Add");
    action_add->setShortcut(Qt::Key_Return);
    action_add->setShortcutContext(Qt::WidgetShortcut);
    connect(action_add, SIGNAL(triggered()), this, SLOT(item_add()));
    this->addAction(action_add);

    action_edit = new QAction("Edit");
    action_edit->setShortcut(Qt::Key_Space);
    action_edit->setShortcutContext(Qt::WidgetShortcut);
    connect(action_edit, SIGNAL(triggered()), this, SLOT(item_edit()));
    this->addAction(action_edit);

    action_delete = new QAction("Delete");
    action_delete->setShortcut(Qt::Key_Delete);
    action_delete->setShortcutContext(Qt::WidgetShortcut);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(item_delete()));
    this->addAction(action_delete);

    action_copy = new QAction("Copy");
    action_copy->setShortcut(QKeySequence(tr("Ctrl+C")));
    action_copy->setShortcutContext(Qt::WidgetShortcut);
    connect(action_copy, SIGNAL(triggered()), this, SLOT(item_copy()));
    this->addAction(action_copy);


    action_paste = new QAction("Paste");
    action_paste->setShortcut(QKeySequence(tr("Ctrl+V")));
    action_paste->setShortcutContext(Qt::WidgetShortcut);
    connect(action_paste, SIGNAL(triggered()), this, SLOT(item_paste()));
    this->addAction(action_paste);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_customContextMenuRequested(const QPoint&)));
}

void EnemyActionTable::item_add()
{
    EnemyActionDialog *dialog = new EnemyActionDialog(ec, this->rowCount(),0,1,100, 1,0,0,1,5,0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int,int,int,int,int,int,int,int)), this, SLOT(change_action(int,int,int,int,int,int,int,int,int,int)));
    dialog->show();
}

void EnemyActionTable::item_edit()
{
    int row = this->currentRow();
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

void EnemyActionTable::item_delete()
{
    int row = this->currentRow();
    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();
    actions.removeAt(row);
    this->ec->obj_set_jsonvalue(RPGDB::ENEMIES, "@actions", actions);

    this->update_actions_from_file();
}

void EnemyActionTable::item_copy()
{
    int row = this->currentRow();
    QJsonObject action = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray().at(row).toObject();
    QJsonDocument doc(action);

    QSettings settings;
    settings.setValue("copied_item_enemy_action", doc.toJson(QJsonDocument::Compact));
}

void EnemyActionTable::item_paste()
{
    int row = this->currentRow();
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
    this->selectRow(row); //TODO Check this
}


void EnemyActionTable::update_actions_from_file()
{
    this->clearContents();
    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();
    QStringList skill_names = this->ec->obj_get_name_list(RPGDB::SKILLS);
    int rowCount = actions.count();
    this->setRowCount(rowCount);


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
            this->setItem(row,0,new QTableWidgetItem(this->basic_actions.at(basic)));
        else if (kind == 1) //Skill
        {
            QStringList skill_names = ec->obj_get_name_list(RPGDB::SKILLS);
            if (skill_id <= skill_names.length())
                this->setItem(row,0,new QTableWidgetItem(skill_names.at(skill_id-1)));
        }
        else this->setItem(row,0,new QTableWidgetItem());
        this->set_readonly(this->item(row,0));

        this->setItem(row,1,new QTableWidgetItem(condition_turns));
        this->set_readonly(this->item(row,0));

        this->setItem(row,2,new QTableWidgetItem(condition_hp < 100 ? QString("%1\% HP or below").arg(condition_hp) : ""));
        this->set_readonly(this->item(row,2));

        this->setItem(row,3,new QTableWidgetItem(condition_level > 1 ? QString("Level %1 or above").arg(condition_level) : ""));
        this->set_readonly(this->item(row,3));

        this->setItem(row,4,new QTableWidgetItem(condition_switch_id > 0 ? QString("Switch [%1] is ON").arg(condition_switch_id,4,10,QChar('0')) : ""));
        this->set_readonly(this->item(row,4));

    }

    this->resizeColumnsToContents();
}


void EnemyActionTable::change_action(int row, int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int skill, int rating, int basic)
{
    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();
    QJsonObject new_action = Factory().create_enemy_action(turn_a, turn_b,hp,level,switch_id,kind,skill,rating,basic);
    actions.removeAt(row);
    actions.insert(row,new_action);
    this->ec->obj_set_jsonvalue(RPGDB::ENEMIES, "@actions", actions);

    this->update_actions_from_file();
    this->selectRow(row); //TODO Check this
}

void EnemyActionTable::on_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *item = this->itemAt(pos);
    if (item) {
        QMenu menu;
        menu.addAction(action_add);
        menu.addAction(action_edit);
        menu.addAction(action_delete);
        menu.addSeparator();
        menu.addAction(action_copy);
        menu.addAction(action_paste);

        menu.exec(this->mapToGlobal(pos));
    }
}


