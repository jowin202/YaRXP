#ifndef EVENTCOMMANDDIALOG_H
#define EVENTCOMMANDDIALOG_H

#include <QWidget>
#include <QListWidget>

class RPGDB;
class RPGMapController;
class RPGMapInfoController;

namespace Ui {
class EventCommandDialog;
}

class EventCommandDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EventCommandDialog(QListWidget *list, RPGDB *db, RPGMapController *mc, RPGMapInfoController *mic, int current, QWidget *parent = nullptr);
    ~EventCommandDialog();

private slots:
    void on_button_show_text_clicked();
    void on_button_show_choices_clicked();
    void on_button_input_number_clicked();
    void on_button_change_text_options_clicked();
    void on_button_button_input_processing_clicked();
    void on_button_wait_clicked();
    void on_button_comment_clicked();
    void on_button_conditional_branch_clicked();
    void on_button_loop_clicked();
    void on_button_break_loop_clicked();
    void on_button_event_processing_clicked();
    void on_button_erase_event_clicked();
    void on_button_call_common_event_clicked();
    void on_button_label_clicked();
    void on_button_jump_to_label_clicked();
    void on_button_control_switches_clicked();
    void on_button_control_variables_clicked();
    void on_button_control_self_switch_clicked();
    void on_button_control_timer_clicked();
    void on_button_change_gold_clicked();
    void on_button_change_items_clicked();
    void on_button_change_weapons_clicked();
    void on_button_change_armor_clicked();
    void on_button_change_party_member_clicked();
    void on_button_change_windowskin_clicked();
    void on_button_change_battle_bgm_clicked();
    void on_button_change_battle_end_me_clicked();
    void on_button_change_save_access_clicked();
    void on_button_change_menu_access_clicked();
    void on_button_change_encounter_clicked();
    void on_button_transfer_player_clicked();
    void on_button_set_event_location_clicked();
    void on_button_scroll_map_clicked();
    void on_button_change_map_settings_clicked();
    void on_button_change_color_fog_tone_clicked();
    void on_button_change_fog_opacity_clicked();
    void on_button_show_animation_clicked();
    void on_button_change_transparent_flag_clicked();
    void on_button_set_move_route_clicked();
    void on_button_wait_for_move_completition_clicked();
    void on_button_prepare_transition_clicked();
    void on_button_execute_transition_clicked();
    void on_button_change_screen_color_tone_clicked();
    void on_button_screen_flash_clicked();
    void on_button_screen_shake_clicked();
    void on_button_show_picture_clicked();
    void on_button_move_picture_clicked();
    void on_button_rotate_picture_clicked();
    void on_button_change_picture_color_tone_clicked();
    void on_button_erase_picture_clicked();
    void on_button_set_weather_effects_clicked();
    void on_button_play_bgm_clicked();
    void on_button_fade_out_bgm_clicked();
    void on_button_play_bgs_clicked();
    void on_button_fade_out_bgs_clicked();
    void on_button_memorize_bgm_clicked();
    void on_button_restore_bgm_clicked();
    void on_button_play_me_clicked();
    void on_button_play_se_clicked();
    void on_button_stop_se_clicked();
    void on_button_battle_processing_clicked();
    void on_battle_shop_processing_clicked();
    void on_battle_input_name_processing_clicked();
    void on_button_change_hp_clicked();
    void on_button_change_sp_clicked();
    void on_button_change_state_clicked();
    void on_button_recover_all_clicked();
    void on_button_change_exp_clicked();
    void on_button_change_level_clicked();
    void on_button_change_parameters_clicked();
    void on_button_change_skills_clicked();
    void on_button_change_equipment_clicked();
    void on_button_change_actor_name_clicked();
    void on_button_change_actor_class_clicked();
    void on_button_change_actor_graphic_clicked();
    void on_button_change_enemy_hp_clicked();
    void on_button_change_enemy_sp_clicked();
    void on_button_change_enemy_state_clicked();
    void on_button_enemy_recover_all_clicked();
    void on_button_enemy_appearance_clicked();
    void on_button_enemy_transform_clicked();
    void on_button_show_battle_animation_clicked();
    void on_button_deal_damage_clicked();
    void on_button_force_action_clicked();
    void on_button_abort_battle_clicked();
    void on_button_call_menu_screen_clicked();
    void on_button_call_save_screen_clicked();
    void on_button_game_over_clicked();
    void on_button_return_to_title_screen_clicked();
    void on_button_script_clicked();

private:
    Ui::EventCommandDialog *ui;
    RPGDB *db;
    RPGMapController *mc;
    RPGMapInfoController *mic;
    QListWidget *list;
    int current;
    int indent = 0;
};

#endif // EVENTCOMMANDDIALOG_H
