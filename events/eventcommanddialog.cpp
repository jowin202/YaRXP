#include "eventcommanddialog.h"
#include "ui_eventcommanddialog.h"

#include "events/eventlistitem.h"
#include "RXIO2/rpgmapcontroller.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/rpgdb.h"
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
#include "commands/buttoninputprocessingdialog.h"

#include "dialogs/audiodialog.h"
#include "dialogs/imagedialog.h"

EventCommandDialog::EventCommandDialog(QListWidget *list, RPGDB *db, RPGMapController *mc, int current, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventCommandDialog)
{
    ui->setupUi(this);
    this->list = list;
    this->db = db;
    this->mc = mc;
    this->mic = new RPGMapInfoController(db);
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
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);

    ChangeTextOptionsDialog *dialog = new ChangeTextOptionsDialog(parameters);
    dialog->show();
    QObject::connect(dialog, &ChangeTextOptionsDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(104,indent,parameters)));
        this->close();
    });

}


void EventCommandDialog::on_button_button_input_processing_clicked()
{
    QJsonArray p;
    p.append(1);
    ButtonInputProcessingDialog *dialog = new ButtonInputProcessingDialog(db, p);
    dialog->show();
    QObject::connect(dialog, &ButtonInputProcessingDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(105,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_wait_clicked()
{
    QString text1 = "Wait";
    QString text2 = "Frames:";

    bool ok;
    int v = QInputDialog::getInt(this, text1, text2, 20,1,999, 1, &ok);
    if (ok){
        QJsonArray newparams;
        newparams.append(v);
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(106,indent,newparams)));
        this->close();
    }
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
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(413,indent,QJsonArray())));
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(0,indent+1,QJsonArray())));
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(112,indent,QJsonArray())));
    this->close();
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
    SingleComboDialog *dialog = new SingleComboDialog(this->db, 117, 1);
    dialog->show();
    QObject::connect(dialog, &SingleComboDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(117,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_label_clicked()
{
    bool ok;
    QString label = QInputDialog::getText(this, "Label", "Label:", QLineEdit::Normal, "", &ok);
    if (ok){
        QJsonArray newparams;
        newparams.append(label);
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(118,indent,newparams)));
        this->close();
    }
}


void EventCommandDialog::on_button_jump_to_label_clicked()
{
    bool ok;
    QString label = QInputDialog::getText(this, "Jump to Label", "Jump to Label:", QLineEdit::Normal, "", &ok);
    if (ok){
        QJsonArray newparams;
        newparams.append(label);
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(119,indent,newparams)));
        this->close();
    }
}


void EventCommandDialog::on_button_control_switches_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(1);
    parameters.append(0);

    ControlSwitchesDialog *dialog = new ControlSwitchesDialog(db,parameters);
    dialog->show();
    QObject::connect(dialog, &ControlSwitchesDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(121,indent,p)));
        this->close();
    });

}


void EventCommandDialog::on_button_control_variables_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(1);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);

    ControlVariablesDialog *dialog = new ControlVariablesDialog(db,mc,parameters);
         dialog->show();
         QObject::connect(dialog, &ControlVariablesDialog::ok_clicked, [=](QJsonArray p) {
             list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(122,indent,p)));
             this->close();
         });
}


void EventCommandDialog::on_button_control_self_switch_clicked()
{
    QJsonArray parameters;
    parameters.append("A");
    parameters.append(0);

    SelfSwitchDialog *dialog = new SelfSwitchDialog(parameters);
    dialog->show();
    connect(dialog, &SelfSwitchDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(123,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_control_timer_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);
    TimerDialog *dialog = new TimerDialog(parameters);
    dialog->show();
    connect(dialog, &TimerDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(124,indent,p)));
        this->close();
    });

}


void EventCommandDialog::on_button_change_gold_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 125, parameters);
    dialog->show();
    connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(125,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_items_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 126, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(126,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_weapons_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 127, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(127,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_armor_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 128, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(128,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_party_member_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(0);
    parameters.append(0);
    ChangePartyMemberDialog *dialog = new ChangePartyMemberDialog(db,parameters);
    dialog->show();
    QObject::connect(dialog, &ChangePartyMemberDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(129,indent,p)));
        this->close();
    });

}


void EventCommandDialog::on_button_change_windowskin_clicked()
{
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::WINDOWSKINS, "");
    dialog->show();
    connect(dialog, &ImageDialog::ok_clicked, [=](QString img) {
        QJsonArray p;
        p.append(img);
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(131,indent,p)));
        this->close();
    });
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
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(mc->get_current_map_id());
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    TransferPlayerDialog *dialog = new TransferPlayerDialog(db,parameters);
    dialog->show();
    connect(dialog, &TransferPlayerDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(201,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_set_event_location_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    SetEventLocationDialog *dialog = new SetEventLocationDialog(db,mc,parameters);
    dialog->show();
    connect(dialog, &SetEventLocationDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(202,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_scroll_map_clicked()
{
    QJsonArray parameters;
    parameters.append(2);
    parameters.append(1);
    parameters.append(4);
    ScrollMapDialog *dialog = new ScrollMapDialog(parameters);
    dialog->show();
    connect(dialog, &ScrollMapDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(203,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_map_settings_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append("");
    parameters.append(0);
    ChangeMapSettingsDialog *dialog = new ChangeMapSettingsDialog(db, parameters);
    dialog->show();
    QObject::connect(dialog, &ChangeMapSettingsDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(204,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_color_fog_tone_clicked()
{
    QJsonArray parameters;
    parameters.append(Factory().create_color_tone(0,0,0,0,true));
    parameters.append(20);
    ChangeColorToneDialog *dialog = new ChangeColorToneDialog(db, 205, parameters);
    dialog->show();
    connect(dialog, &ChangeColorToneDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(205,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_fog_opacity_clicked()
{
    QJsonArray parameters;
    parameters.append(64);
    parameters.append(20);
    SpinSpinDialog *dialog = new SpinSpinDialog(206, parameters);
    dialog->show();
    QObject::connect(dialog, &SpinSpinDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(206,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_show_animation_clicked()
{
    QJsonArray parameters;
    parameters.append(-1);
    parameters.append(1);
    ComboComboDialog *dialog = new ComboComboDialog(db, mc, 207, parameters);
    dialog->show();
    QObject::connect(dialog, &ComboComboDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(207,indent,p)));
        this->close();
    });
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
    QJsonArray parameters;
    parameters.append(-1); // event = player

    QJsonObject moveroute = Factory().create_move_route(false, false);
    parameters.append(moveroute);

    MoveRouteDialog *dialog = new MoveRouteDialog(db, mc, parameters);
    dialog->show();
    connect(dialog, &MoveRouteDialog::ok_clicked, [=](QJsonArray p){
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(209,indent,p)));


        //create new
        //-2 because ignoring the last one (empty)
        for (int i = p.at(1).toObject().value("@list").toArray().count()-2; i >= 0; i--)
        {
            QJsonArray array;
            array.append(p.at(1).toObject().value("@list").toArray().at(i).toObject());
            list->insertItem(current+1,new EventListItem(list,mc,mic,Factory().create_event_command(509,indent,array)));
        }

        this->close();
    });
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
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::TRANSITIONS, "");
    dialog->show();
    connect(dialog, &ImageDialog::ok_clicked, [=](QString img) {
        QJsonArray p;
        p.append(img);
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(222,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_screen_color_tone_clicked()
{
    QJsonArray parameters;
    parameters.append(Factory().create_color_tone(0,0,0,0,true));
    parameters.append(20);
    ChangeColorToneDialog *dialog = new ChangeColorToneDialog(db, 223, parameters);
    dialog->show();
    connect(dialog, &ChangeColorToneDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(223,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_screen_flash_clicked()
{
    QJsonArray parameters;
    parameters.append(Factory().create_color_tone(255,255,255,255,false));
    parameters.append(10);
    ChangeColorToneDialog *dialog = new ChangeColorToneDialog(db, 224, parameters);
    dialog->show();
    connect(dialog, &ChangeColorToneDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(224,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_screen_shake_clicked()
{
    QJsonArray parameters;
    parameters.append(5);
    parameters.append(5);
    parameters.append(5);
    ScreenShakeDialog *dialog = new ScreenShakeDialog(parameters);
    dialog->show();
    connect(dialog, &ScreenShakeDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(225,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_show_picture_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append("");
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(100);
    parameters.append(100);
    parameters.append(255);
    parameters.append(0);

    PictureDialog *dialog = new PictureDialog(db, 231, parameters);
    dialog->show();
    connect(dialog, &PictureDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(231,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_move_picture_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(20);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(100);
    parameters.append(100);
    parameters.append(255);
    parameters.append(0);

    PictureDialog *dialog = new PictureDialog(db, 232, parameters);
    dialog->show();
    connect(dialog, &PictureDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(232,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_rotate_picture_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(5);
    SpinSpinDialog *dialog = new SpinSpinDialog(233, parameters);
    dialog->show();
    QObject::connect(dialog, &SpinSpinDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(233,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_picture_color_tone_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(Factory().create_color_tone(0,0,0,0,true));
    parameters.append(20);
    ChangeColorToneDialog *dialog = new ChangeColorToneDialog(db, 234, parameters);
    dialog->show();
    connect(dialog, &ChangeColorToneDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(234,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_erase_picture_clicked()
{
    QString text1 = "Erase Picture";
    QString text2 = "Pic. No.:";

    bool ok;
    int v = QInputDialog::getInt(this, text1, text2, 1,1,999, 1, &ok);
    if (ok){
        QJsonArray newparams;
        newparams.append(v);
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(235,indent,newparams)));
        this->close();
    }
}


void EventCommandDialog::on_button_set_weather_effects_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(5);
    parameters.append(0);
    WeatherDialog *dialog = new WeatherDialog(parameters);
    dialog->show();
    QObject::connect(dialog, &WeatherDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(236,indent,p)));
        this->close();
    });
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
    QString text1 = "Fade Out BGM";
    QString text2 = "Time in sec:";

    bool ok;
    int v = QInputDialog::getInt(this, text1, text2, 10,1,999, 1, &ok);
    if (ok){
        QJsonArray newparams;
        newparams.append(v);
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(242,indent,newparams)));
        this->close();
    }
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
    QString text1 = "Fade Out BGS";
    QString text2 = "Time in sec:";

    bool ok;
    int v = QInputDialog::getInt(this, text1, text2, 10,1,999, 1, &ok);
    if (ok){
        QJsonArray newparams;
        newparams.append(v);
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(246,indent,newparams)));
        this->close();
    }
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
    list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(251,indent,QJsonArray())));
    this->close();
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
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    parameters.append(false);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 311, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(311,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_sp_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 312, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(312,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_state_clicked()
{
    QJsonArray p;
    p.append(0);
    p.append(0);
    p.append(1);
    ChangeStateDialog *dialog = new ChangeStateDialog(db, 313, p);
    dialog->show();
    QObject::connect(dialog, &ChangeStateDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(313,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_recover_all_clicked()
{
    SingleComboDialog *dialog = new SingleComboDialog(this->db, 314, 0);
    dialog->show();
    QObject::connect(dialog, &SingleComboDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(314,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_exp_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 315, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(315,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_level_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 316, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(316,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_parameters_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 317, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(317,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_skills_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(0);
    parameters.append(1);
    ChangeStateDialog *dialog = new ChangeStateDialog(db, 318, parameters);
    dialog->show();
    QObject::connect(dialog, &ChangeStateDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(318,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_equipment_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(0);
    parameters.append(0);
    ChangeEquipmentDialog *dialog = new ChangeEquipmentDialog(db, parameters);
    dialog->show();
    connect(dialog, &ChangeEquipmentDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(319,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_actor_name_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append("");
    ChangeActorNameDialog *dialog = new ChangeActorNameDialog(db, parameters);
         dialog->show();
         QObject::connect(dialog, &ChangeActorNameDialog::ok_clicked, [=](QJsonArray p) {
             list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(320,indent,p)));
             this->close();
         });

}


void EventCommandDialog::on_button_change_actor_class_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append(1);
    ComboComboDialog *dialog = new ComboComboDialog(db, mc, 321, parameters);
    dialog->show();
    QObject::connect(dialog, &ComboComboDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(321,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_actor_graphic_clicked()
{
    QJsonArray parameters;
    parameters.append(1);
    parameters.append("");
    parameters.append(0);
    parameters.append("");
    parameters.append(0);
    ChangeActorGraphicDialog *dialog = new ChangeActorGraphicDialog(db, parameters);
    dialog->show();
    QObject::connect(dialog, &ChangeActorGraphicDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(322,indent,p)));
        this->close();
    });
}

void EventCommandDialog::on_button_change_enemy_hp_clicked()
{
    QJsonArray parameters;
    parameters.append(-1);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    parameters.append(false);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 331, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(331,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_enemy_sp_clicked()
{
    QJsonArray parameters;
    parameters.append(-1);
    parameters.append(0);
    parameters.append(0);
    parameters.append(1);
    IncreaseDecreaseDialog *dialog = new IncreaseDecreaseDialog(this->db, 332, parameters);
    dialog->show();
    QObject::connect(dialog, &IncreaseDecreaseDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(332,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_change_enemy_state_clicked()
{
    QJsonArray p;
    p.append(-1);
    p.append(0);
    p.append(1);
    ChangeStateDialog *dialog = new ChangeStateDialog(db, 333, p);
    dialog->show();
    QObject::connect(dialog, &ChangeStateDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(333,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_enemy_recover_all_clicked()
{
    SingleComboDialog *dialog = new SingleComboDialog(this->db, 334, -1);
    dialog->show();
    QObject::connect(dialog, &SingleComboDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(334,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_enemy_appearance_clicked()
{
    SingleComboDialog *dialog = new SingleComboDialog(this->db, 335, 0);
    dialog->show();
    QObject::connect(dialog, &SingleComboDialog::ok_clicked, [=](QJsonArray parameters) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(335,indent,parameters)));
        this->close();
    });
}


void EventCommandDialog::on_button_enemy_transform_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(1);
    ComboComboDialog *dialog = new ComboComboDialog(db, mc, 336, parameters);
    dialog->show();
    QObject::connect(dialog, &ComboComboDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(336,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_show_battle_animation_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(-1);
    parameters.append(1);
    ShowBattleAnimationDialog *dialog = new ShowBattleAnimationDialog(db, parameters);
    dialog->show();
    connect(dialog, &ShowBattleAnimationDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(337,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_deal_damage_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(-1);
    parameters.append(0);
    parameters.append(1);
    DealDamageDialog *dialog = new DealDamageDialog(db, parameters);
    dialog->show();
    connect(dialog, &DealDamageDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(338,indent,p)));
        this->close();
    });
}


void EventCommandDialog::on_button_force_action_clicked()
{
    QJsonArray parameters;
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(0);
    parameters.append(-1);
    parameters.append(0);
    ForceActionDialog *dialog = new ForceActionDialog(db, parameters);
    dialog->show();
    connect(dialog, &ForceActionDialog::ok_clicked, [=](QJsonArray p) {
        list->insertItem(current, new EventListItem(list,mc,mic,Factory().create_event_command(339,indent,p)));
        this->close();
    });
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

