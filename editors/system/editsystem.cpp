#include "editsystem.h"
#include "ui_editsystem.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

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
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@windowskin_name", this->ui->line_windowskin_graphic);
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@title_name", this->ui->line_title_graphic);
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@gameover_name", this->ui->line_gameover_graphic);
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@battle_transition", this->ui->line_battle_transition_graphic);

    /*
     * //TODO audiofiles
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@title_bgm", this->ui->line_title_bgm);
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@battle_bgm", this->ui->line_battle_bgm);
    this->ec->connect_string_to_text_field(RPGDB::SYSTEM, "@battle_end_me", this->ui->line_battle_end_me);
    */


}
