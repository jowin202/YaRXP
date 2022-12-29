#include "editsystem.h"
#include "ui_editsystem.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgeditorcontroller.h"

#include "dialogs/imagedialog.h"
#include "dialogs/audiodialog.h"



EditSystem::EditSystem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditSystem)
{
    ui->setupUi(this);
}

EditSystem::~EditSystem()
{
    delete ui;
}

void EditSystem::setEC(RPGEditorController *ec)
{
    this->ec = ec;

    //graphic files can be directly connected to variables
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@windowskin_name", this->ui->line_windowskin_graphic);
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@title_name", this->ui->line_title_graphic);
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@gameover_name", this->ui->line_gameover_graphic);
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@battle_transition", this->ui->line_battle_transition_graphic);

    connect(this->ec, &RPGEditorController::current_system_changed, this, [=]() {
        QJsonObject words = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject();
        this->ui->line_currency->setText(words.value("@gold").toString());
        this->ui->line_weapon->setText(words.value("@weapon").toString());

        this->ui->line_hp->setText(words.value("@hp").toString());
        this->ui->line_shield->setText(words.value("@armor1").toString());

        this->ui->line_sp->setText(words.value("@sp").toString());
        this->ui->line_helmet->setText(words.value("@armor2").toString());

        this->ui->line_str->setText(words.value("@str").toString());
        this->ui->line_armor->setText(words.value("@armor3").toString());

        this->ui->line_dex->setText(words.value("@dex").toString());
        this->ui->line_accessory->setText(words.value("@armor4").toString());

        this->ui->line_agi->setText(words.value("@agi").toString());
        this->ui->line_attack->setText(words.value("@attack").toString());

        this->ui->line_int->setText(words.value("@int").toString());
        this->ui->line_skill->setText(words.value("@skill").toString());

        this->ui->line_atk->setText(words.value("@atk").toString());
        this->ui->line_defend->setText(words.value("@guard").toString());

        this->ui->line_pdef->setText(words.value("@pdef").toString());
        this->ui->line_item->setText(words.value("@item").toString());

        this->ui->line_mdef->setText(words.value("@mdef").toString());
        this->ui->line_equip->setText(words.value("@equip").toString());

        this->ui->line_title_bgm->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@title_bgm").toObject().value("@name").toString());
        this->ui->line_battle_bgm->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battle_bgm").toObject().value("@name").toString());
        this->ui->line_battle_end_me->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battle_end_me").toObject().value("@name").toString());
        this->ui->line_gameover_me->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@gameover_me").toObject().value("@name").toString());
        this->ui->line_cursor_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@cursor_se").toObject().value("@name").toString());
        this->ui->line_decision_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@decision_se").toObject().value("@name").toString());
        this->ui->line_cancel_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@cancel_se").toObject().value("@name").toString());
        this->ui->line_buzzer_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@buzzer_se").toObject().value("@name").toString());
        this->ui->line_equip_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@equip_se").toObject().value("@name").toString());
        this->ui->line_shop_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@shop_se").toObject().value("@name").toString());
        this->ui->line_save_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@save_se").toObject().value("@name").toString());
        this->ui->line_load_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@load_se").toObject().value("@name").toString());
        this->ui->line_battle_start_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battle_start_se").toObject().value("@name").toString());
        this->ui->line_escape_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@escape_se").toObject().value("@name").toString());
        this->ui->line_actor_collapse_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@actor_collapse_se").toObject().value("@name").toString());
        this->ui->line_enemy_collapse_se->setText(this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@enemy_collapse_se").toObject().value("@name").toString());

        this->ec->fill_list(this->ui->element_list, RPGDB::ELEMENTS, true, 3, false);
        this->update_party_members();
    });

    connect(this->ui->line_currency, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@gold", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_weapon, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@weapon", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_hp, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@hp", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_shield, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@armor1", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_sp, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@sp", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_helmet, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@armor2", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_str, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@str", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_armor, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@armor3", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_dex, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@dex", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_accessory, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@armor4", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_agi, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@agi", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_attack, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@attack", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_int, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@int", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_skill, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@skill", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_atk, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@atk", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_defend, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@guard", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_pdef, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@pdef", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_item, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@item", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});

    connect(this->ui->line_mdef, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@mdef", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
    connect(this->ui->line_equip, &QLineEdit::textChanged, this, [=](QString val){ QJsonObject value = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@words").toObject(); value.insert("@equip", val); this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@words", value);});
}

void EditSystem::on_button_windowskin_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::WINDOWSKINS, this->ui->line_windowskin_graphic->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_windowskin_graphic, SLOT(setText(QString)));
    dialog->show();
}



void EditSystem::on_button_title_graphic_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::TITLES, this->ui->line_title_graphic->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_title_graphic, SLOT(setText(QString)));
    dialog->show();
}


void EditSystem::on_button_gameover_graphic_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::GAMEOVERS, this->ui->line_gameover_graphic->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_gameover_graphic, SLOT(setText(QString)));
    dialog->show();
}


void EditSystem::on_button_battle_transistion_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::TRANSITIONS, this->ui->line_battle_transition_graphic->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_battle_transition_graphic, SLOT(setText(QString)));
    dialog->show();
}


void EditSystem::on_button_title_bgm_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@title_bgm").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::BGM);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@title_bgm",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_battle_bgm_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battle_bgm").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::BGM);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@battle_bgm",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_battle_end_me_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battle_end_me").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::ME);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@battle_end_me",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_gameover_me_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@gameover_me").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::ME);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@gameover_me",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_cursor_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@cursor_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@cursor_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_decision_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@decision_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@decision_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_cancel_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@cancel_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@cancel_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_buzzer_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@buzzer_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@buzzer_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_equip_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@equip_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@equip_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_shop_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@shop_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@shop_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_save_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@save_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@save_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_load_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@load_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@load_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_battle_start_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battle_start_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@battle_start_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_escape_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@escape_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@escape_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_actor_collapse_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@actor_collapse_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@actor_collapse_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_button_enemy_collapse_se_clicked()
{
    QJsonObject audiofile = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@enemy_collapse_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), audiofile.value("@name").toString(), audiofile.value("@volume").toInt(), audiofile.value("@pitch").toInt(), AudioDialog::SE);
    connect(dialog, &AudioDialog::ok_clicked, this, [=](QString n,int v,int p) { this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@enemy_collapse_se",Factory().create_audiofile(n,v,p)); this->ec->refresh(RPGDB::SYSTEM); });
    dialog->show();
}


void EditSystem::on_element_list_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    bool ok;
    QJsonArray element_list = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@elements").toArray();
    QString element_name = QInputDialog::getText(this, "Element Name", "New Element Name:", QLineEdit::Normal, element_list.at(this->ui->element_list->currentRow()+1).toString(), &ok);
    if (ok)
    {
        element_list.removeAt(this->ui->element_list->currentRow()+1);
        element_list.insert(this->ui->element_list->currentRow()+1, element_name);
        this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@elements", element_list);
        this->ec->fill_list(this->ui->element_list, RPGDB::ELEMENTS, true, 3, false);
    }
}

void EditSystem::update_party_members()
{
    QJsonArray party_members = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@party_members").toArray();
    for (int i = 0; i < party_members.count(); i++)
    {
        this->ui->party_member_list->addItem(QString("%1: ").arg(party_members.at(i).toInt(),3,10, QChar('0')) + this->ec->get_object_by_id(RPGDB::ACTORS,party_members.at(i).toInt()).value("@name").toString());
        //this->ui->test_battler_list->addItem()
    }
}

void EditSystem::on_button_element_max_clicked()
{
    bool ok;
    QJsonArray element_list = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@elements").toArray();
    int max = QInputDialog::getInt(this, "Max. Elements", "Max. Elements:", element_list.count()-1,1,999,1,&ok);
    if (ok)
    {
        while (element_list.count() > max+1)
            element_list.pop_back();
        while (element_list.count() <= max)
            element_list.append("");
        this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@elements", element_list);
        this->ec->fill_list(this->ui->element_list, RPGDB::ELEMENTS, true, 3, false);
    }
}



void EditSystem::on_party_member_list_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
}

