#ifndef MOVEROUTEDIALOG_H
#define MOVEROUTEDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>
#include <QInputDialog>

#include <QAction>

class RPGDB;
class RPGMapController;

namespace Ui {
class MoveRouteDialog;
}

class MoveRouteDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MoveRouteDialog(RPGDB *db, RPGMapController *mc, QJsonArray parameters, QWidget *parent = nullptr);
    ~MoveRouteDialog();
    void fill_list();


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(QJsonArray);

private slots:
    void on_button_move_down_clicked();
    void on_button_move_left_clicked();
    void on_button_move_right_clicked();
    void on_button_move_up_clicked();
    void on_button_move_lower_left_clicked();
    void on_button_move_lower_right_clicked();
    void on_button_move_upper_left_clicked();
    void on_button_move_upper_right_clicked();
    void on_button_move_random_clicked();
    void on_button_move_toward_player_clicked();
    void on_button_move_away_from_player_clicked();
    void on_button_one_step_forward_clicked();
    void on_button_one_step_backward_clicked();
    void on_button_jump_clicked();
    void on_button_wait_clicked();

    void on_button_turn_down_clicked();
    void on_button_turn_left_clicked();
    void on_button_turn_right_clicked();
    void on_button_turn_up_clicked();
    void on_button_turn_90_right_clicked();
    void on_button_turn_90_left_clicked();
    void on_button_turn_180_clicked();
    void on_button_turn_90_left_or_right_clicked();
    void on_button_turn_random_clicked();
    void on_button_turn_toward_player_clicked();
    void on_button_turn_away_from_player_clicked();
    void on_button_switch_on_clicked();
    void on_button_switch_off_clicked();
    void on_button_change_speed_clicked();
    void on_button_change_freq_clicked();

    void on_button_move_animation_on_clicked();
    void on_button_move_animation_off_clicked();
    void on_button_stop_animation_on_clicked();
    void on_button_stop_animation_off_clicked();
    void on_button_direction_fix_on_clicked();
    void on_button_direction_fix_off_clicked();
    void on_button_through_on_clicked();
    void on_button_through_off_clicked();
    void on_button_always_on_top_on_clicked();
    void on_button_always_on_top_off_clicked();
    void on_button_change_graphic_clicked();
    void on_button_opacity_clicked();
    void on_button_change_blending_clicked();
    void on_button_play_se_clicked();
    void on_button_script_clicked();


    void on_button_cancel_clicked();

    void on_button_ok_clicked();

    void do_delete();

private:
    Ui::MoveRouteDialog *ui;
    RPGDB *db;
    QJsonArray list;

    QAction action_delete;

    QStringList text_blend { "Normal", "Add", "Sub" };
    QStringList text_move_routes = { "", "Move Down", "Move Left", "Move Right", "Move Up",
                                     "Move Lower Left", "Move Lower Right", "Move Upper Left", "Move Upper Right",
                                   "Move at Random", "Move toward Player", "Move away from Player", "1 Step Forward", "1 Step Backward",
                                   "Jump: ", "Wait: ",

                                   "Turn Down", "Turn Left", "Turn Right", "Turn Up", "Turn 90° Right", "Turn 90° Left", "Turn 180°",
                                   "Turn 90° Right or Left", "Turn at Random", "Turn toward Player", "Turn away from Player",
                                   "Switch ON: ", "Switch OFF: ", "Change Speed: ", "Change Freq: ",

                                   "Move Animation ON", "Move Animation OFF", "Stop Animation ON", "Stop Animation OFF", "Direction Fix ON", "Direction Fix OFF",
                                   "Through ON", "Through OFF", "Always on Top ON", "Always on Top OFF", "Graphic: ",
                                   "Change Opacity: ", "Change Blending: ", "SE: ", "Script: " };
};

#endif // MOVEROUTEDIALOG_H
