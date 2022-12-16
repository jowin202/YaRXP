#ifndef EDITSYSTEM_H
#define EDITSYSTEM_H

#include <QWidget>

namespace Ui {
class EditSystem;
}

class RPGEditorController;

class EditSystem : public QWidget
{
    Q_OBJECT

public:
    explicit EditSystem(QWidget *parent = nullptr);
    ~EditSystem();

    void setEC(RPGEditorController *ec);

private slots:
    void on_button_windowskin_clicked();
    void on_button_title_graphic_clicked();
    void on_button_gameover_graphic_clicked();
    void on_button_battle_transistion_clicked();


    void on_button_title_bgm_clicked();
    void on_button_battle_bgm_clicked();
    void on_button_battle_end_me_clicked();
    void on_button_gameover_me_clicked();
    void on_button_cursor_se_clicked();
    void on_button_decision_se_clicked();
    void on_button_cancel_se_clicked();
    void on_button_buzzer_se_clicked();
    void on_button_equip_se_clicked();
    void on_button_shop_se_clicked();
    void on_button_save_se_clicked();
    void on_button_load_se_clicked();
    void on_button_battle_start_se_clicked();
    void on_button_escape_se_clicked();
    void on_button_actor_collapse_se_clicked();
    void on_button_enemy_collapse_se_clicked();

private:
    Ui::EditSystem *ui;
    RPGEditorController *ec;
};

#endif // EDITSYSTEM_H
