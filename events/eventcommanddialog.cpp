#include "eventcommanddialog.h"
#include "ui_eventcommanddialog.h"

#include "events/eventlistitem.h"
#include "RXIO2/rpgmapcontroller.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/factory.h"


#include "commands/changetextoptionsdialog.h"
#include "commands/showtextdialog.h"
#include "commands/radiodialog.h"
#include "commands/singlecombodialog.h"
#include "commands/increasedecreasedialog.h"
#include "commands/changestatedialog.h"
#include "commands/combospindialog.h"
#include "commands/changecolortonedialog.h"
#include "commands/combocombodialog.h"
#include "commands/weatherdialog.h"
#include "commands/picturedialog.h"
#include "commands/conditionalbranchdialog.h"
#include "commands/spinspindialog.h"
#include "commands/changemapsettingsdialog.h"
#include "commands/changeequipmentdialog.h"
#include "commands/timerdialog.h"
#include "commands/selfswitchdialog.h"
#include "commands/scrollmapdialog.h"
#include "commands/screenshakedialog.h"
#include "commands/forceactiondialog.h"
#include "commands/dealdamagedialog.h"
#include "commands/showbattleanimationdialog.h"
#include "commands/changeactornamedialog.h"
#include "commands/shopprocessingdialog.h"
#include "commands/changeactorgraphicdialog.h"
#include "commands/changepartymemberdialog.h"
#include "commands/controlswitchesdialog.h"
#include "commands/controlvariablesdialog.h"
#include "commands/choicesdialog.h"
#include "commands/transferplayerdialog.h"
#include "commands/seteventlocationdialog.h"
#include "commands/battleprocessingdialog.h"
#include "commands/moveroutedialog.h"

#include "dialogs/audiodialog.h"
#include "dialogs/imagedialog.h"

EventCommandDialog::EventCommandDialog(QListWidget *list, RPGDB *db, RPGMapController *mc, RPGMapInfoController *mic, int current, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventCommandDialog)
{
    ui->setupUi(this);
    this->list = list;
    this->db = db;
    this->mc = mc;
    this->mic = mic;
    this->current = current;

    if (dynamic_cast<EventListItem*>(list->item(current)) != nullptr)
    {
        this->indent = ((EventListItem*)list->item(current))->get_obj().value("@indent").toInt();

        //TODO
    }

}

EventCommandDialog::~EventCommandDialog()
{
    delete ui;
}

void EventCommandDialog::on_button_show_text_clicked()
{
    ShowTextDialog *dialog = new ShowTextDialog(nullptr, 101);
    dialog->setString("");
    dialog->show();

    connect(dialog, &ShowTextDialog::ok_clicked, [=](QString text){
        QStringList list = text.split('\n');
        if (list.last() == "")
            list.removeLast();

        while (list.length() > 1)
        {
            QJsonArray p;
            p.append(list.last());
            list.pop_back();
            QJsonObject obj = Factory().create_event_command(401, indent ,p);
            this->list->insertItem(current, new EventListItem(this->list,mc,mic,obj));
        }
        QJsonArray p;
        p.append(list.first());
        list.pop_front();
        QJsonObject obj = Factory().create_event_command(101, indent ,p);
        this->list->insertItem(current, new EventListItem(this->list,mc,mic,obj));

        this->close();
    });
}


void EventCommandDialog::on_button_show_choices_clicked()
{
}


void EventCommandDialog::on_button_input_number_clicked()
{

}


void EventCommandDialog::on_button_change_text_options_clicked()
{

}


void EventCommandDialog::on_button_button_input_processing_clicked()
{

}


void EventCommandDialog::on_button_wait_clicked()
{

}


void EventCommandDialog::on_button_comment_clicked()
{
    ShowTextDialog *dialog = new ShowTextDialog(nullptr, 108);
    dialog->setString("");
    dialog->show();

    connect(dialog, &ShowTextDialog::ok_clicked, [=](QString text){
        QStringList list = text.split('\n');
        if (list.last() == "")
            list.removeLast();

        while (list.length() > 1)
        {
            QJsonArray p;
            p.append(list.last());
            list.pop_back();
            QJsonObject obj = Factory().create_event_command(408, indent ,p);
            this->list->insertItem(current, new EventListItem(this->list,mc,mic,obj));
        }
        QJsonArray p;
        p.append(list.first());
        list.pop_front();
        QJsonObject obj = Factory().create_event_command(108, indent ,p);
        this->list->insertItem(current, new EventListItem(this->list,mc,mic,obj));

        this->close();
    });
}


void EventCommandDialog::on_button_conditional_branch_clicked()
{

}


void EventCommandDialog::on_button_loop_clicked()
{

}


void EventCommandDialog::on_button_break_loop_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(113,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_event_processing_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(115,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_erase_event_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(116,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_call_common_event_clicked()
{

}


void EventCommandDialog::on_button_label_clicked()
{

}


void EventCommandDialog::on_button_jump_to_label_clicked()
{

}


void EventCommandDialog::on_button_control_switches_clicked()
{

}


void EventCommandDialog::on_button_control_variables_clicked()
{

}


void EventCommandDialog::on_button_control_self_switch_clicked()
{

}


void EventCommandDialog::on_button_control_timer_clicked()
{

}


void EventCommandDialog::on_button_change_gold_clicked()
{

}


void EventCommandDialog::on_button_change_items_clicked()
{

}


void EventCommandDialog::on_button_change_weapons_clicked()
{

}


void EventCommandDialog::on_button_change_armor_clicked()
{

}


void EventCommandDialog::on_button_change_party_member_clicked()
{

}


void EventCommandDialog::on_button_change_windowskin_clicked()
{

}


void EventCommandDialog::on_button_change_battle_bgm_clicked()
{
    int type = AudioDialog::BGM;
    AudioDialog *dialog = new AudioDialog(this->db, "", 100, 100, type);
    dialog->show();
    QObject::connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
        QJsonArray param;
        param.append(Factory().create_audiofile(name, volume,pitch));
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(132,indent,param)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_battle_end_me_clicked()
{
    int type = AudioDialog::ME;
    AudioDialog *dialog = new AudioDialog(this->db, "", 100, 100, type);
    dialog->show();
    QObject::connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
        QJsonArray param;
        param.append(Factory().create_audiofile(name, volume,pitch));
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(133,indent,param)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_save_access_clicked()
{
    RadioDialog *dialog = new RadioDialog(nullptr, 134, true);
    dialog->show();
    connect(dialog, &RadioDialog::ok_clicked, [=](QJsonArray array){
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(134,indent,array)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_menu_access_clicked()
{
    RadioDialog *dialog = new RadioDialog(nullptr, 135, true);
    dialog->show();
    connect(dialog, &RadioDialog::ok_clicked, [=](QJsonArray array){
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(135,indent,array)));
        this->close();
    });

}


void EventCommandDialog::on_button_change_encounter_clicked()
{
    RadioDialog *dialog = new RadioDialog(nullptr, 136, true);
    dialog->show();
    connect(dialog, &RadioDialog::ok_clicked, [=](QJsonArray array){
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(136,indent,array)));
        this->close();
    });

}


void EventCommandDialog::on_button_transfer_player_clicked()
{

}


void EventCommandDialog::on_button_set_event_location_clicked()
{

}


void EventCommandDialog::on_button_scroll_map_clicked()
{

}


void EventCommandDialog::on_button_change_map_settings_clicked()
{

}


void EventCommandDialog::on_button_change_color_fog_tone_clicked()
{

}


void EventCommandDialog::on_button_change_fog_opacity_clicked()
{

}


void EventCommandDialog::on_button_show_animation_clicked()
{

}


void EventCommandDialog::on_button_change_transparent_flag_clicked()
{
    RadioDialog *dialog = new RadioDialog(nullptr, 208, true);
    dialog->show();
    connect(dialog, &RadioDialog::ok_clicked, [=](QJsonArray array){
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(208,indent,array)));
        this->close();
    });

}


void EventCommandDialog::on_button_set_move_route_clicked()
{

}


void EventCommandDialog::on_button_wait_for_move_completition_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(210,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_prepare_transition_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(221,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_execute_transition_clicked()
{

}


void EventCommandDialog::on_button_change_screen_color_tone_clicked()
{

}


void EventCommandDialog::on_button_screen_flash_clicked()
{

}


void EventCommandDialog::on_button_screen_shake_clicked()
{

}


void EventCommandDialog::on_button_show_picture_clicked()
{

}


void EventCommandDialog::on_button_move_picture_clicked()
{

}


void EventCommandDialog::on_button_rotate_picture_clicked()
{

}


void EventCommandDialog::on_button_change_picture_color_tone_clicked()
{

}


void EventCommandDialog::on_button_erase_picture_clicked()
{

}


void EventCommandDialog::on_button_set_weather_effects_clicked()
{

}


void EventCommandDialog::on_button_play_bgm_clicked()
{
    int type = AudioDialog::BGM;
    AudioDialog *dialog = new AudioDialog(this->db, "", 100, 100, type);
    dialog->show();
    QObject::connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
        QJsonArray param;
        param.append(Factory().create_audiofile(name, volume,pitch));
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(241,indent,param)));
        this->close();
    });
}


void EventCommandDialog::on_button_fade_out_bgm_clicked()
{

}


void EventCommandDialog::on_button_play_bgs_clicked()
{
    int type = AudioDialog::BGS;
    AudioDialog *dialog = new AudioDialog(this->db, "", 100, 100, type);
    dialog->show();
    QObject::connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
        QJsonArray param;
        param.append(Factory().create_audiofile(name, volume,pitch));
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(245,indent,param)));
        this->close();
    });
}


void EventCommandDialog::on_button_fade_out_bgs_clicked()
{

}


void EventCommandDialog::on_button_memorize_bgm_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(247,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_restore_bgm_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(248,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_play_me_clicked()
{
    int type = AudioDialog::ME;
    AudioDialog *dialog = new AudioDialog(this->db, "", 100, 100, type);
    dialog->show();
    QObject::connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
        QJsonArray param;
        param.append(Factory().create_audiofile(name, volume,pitch));
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(249,indent,param)));
        this->close();
    });
}


void EventCommandDialog::on_button_play_se_clicked()
{
    int type = AudioDialog::SE;
    AudioDialog *dialog = new AudioDialog(this->db, "", 100, 100, type);
    dialog->show();
    QObject::connect(dialog, &AudioDialog::ok_clicked, [=](QString name, int volume, int pitch){
        QJsonArray param;
        param.append(Factory().create_audiofile(name, volume,pitch));
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(250,indent,param)));
        this->close();
    });
}


void EventCommandDialog::on_button_stop_se_clicked()
{

}


void EventCommandDialog::on_button_battle_processing_clicked()
{

}


void EventCommandDialog::on_battle_shop_processing_clicked()
{

}


void EventCommandDialog::on_battle_input_name_processing_clicked()
{

}


void EventCommandDialog::on_button_change_hp_clicked()
{

}


void EventCommandDialog::on_button_change_sp_clicked()
{

}


void EventCommandDialog::on_button_change_state_clicked()
{

}


void EventCommandDialog::on_button_recover_all_clicked()
{

}


void EventCommandDialog::on_button_change_exp_clicked()
{

}


void EventCommandDialog::on_button_change_level_clicked()
{

}


void EventCommandDialog::on_button_change_parameters_clicked()
{

}


void EventCommandDialog::on_button_change_skills_clicked()
{

}


void EventCommandDialog::on_button_change_equipment_clicked()
{

}


void EventCommandDialog::on_button_change_actor_name_clicked()
{

}


void EventCommandDialog::on_button_change_actor_class_clicked()
{

}


void EventCommandDialog::on_button_change_actor_graphic_clicked()
{

}

void EventCommandDialog::on_button_change_enemy_hp_clicked()
{

}


void EventCommandDialog::on_button_change_enemy_sp_clicked()
{

}


void EventCommandDialog::on_button_change_enemy_state_clicked()
{

}


void EventCommandDialog::on_button_enemy_recover_all_clicked()
{

}


void EventCommandDialog::on_button_enemy_appearance_clicked()
{

}


void EventCommandDialog::on_button_enemy_transform_clicked()
{

}


void EventCommandDialog::on_button_show_battle_animation_clicked()
{

}


void EventCommandDialog::on_button_deal_damage_clicked()
{

}


void EventCommandDialog::on_button_force_action_clicked()
{
}


void EventCommandDialog::on_button_abort_battle_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(340,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_call_menu_screen_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(351,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_call_save_screen_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(352,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_game_over_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(353,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_return_to_title_screen_clicked()
{
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(354,indent,QJsonArray())));
    this->close();
}


void EventCommandDialog::on_button_script_clicked()
{   
    ShowTextDialog *dialog = new ShowTextDialog(nullptr, 355);
    dialog->setString("");
    dialog->show();

    connect(dialog, &ShowTextDialog::ok_clicked, [=](QString text){
        QStringList list = text.split('\n');
        if (list.last() == "")
            list.removeLast();

        while (list.length() > 1)
        {
            QJsonArray p;
            p.append(list.last());
            list.pop_back();
            QJsonObject obj = Factory().create_event_command(655, indent ,p);
            this->list->insertItem(current, new EventListItem(this->list,mc,mic,obj));
        }
        QJsonArray p;
        p.append(list.first());
        list.pop_front();
        QJsonObject obj = Factory().create_event_command(355, indent ,p);
        this->list->insertItem(current, new EventListItem(this->list,mc,mic,obj));

        this->close();
    });
}

