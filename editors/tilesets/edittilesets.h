#ifndef EDITTILESETS_H
#define EDITTILESETS_H

#include <QWidget>
#include <QActionGroup>
#include <QTimer>

namespace Ui {
class EditTilesets;
}

class RPGEditorController;
class EditTilesets : public QWidget
{
    Q_OBJECT

public:
    explicit EditTilesets(QWidget *parent = nullptr);
    ~EditTilesets();

    void setEC(RPGEditorController *ec);
    void focus_on_tile(int y_value);


private slots:
    void on_button_choose_tileset_clicked();
    void fill_autotile_names();
    void save_changed_autotile_names();

    void on_button_autotile_1_clicked();
    void on_button_autotile_2_clicked();
    void on_button_autotile_3_clicked();
    void on_button_autotile_4_clicked();
    void on_button_autotile_5_clicked();
    void on_button_autotile_6_clicked();
    void on_button_autotile_7_clicked();

    void on_button_panorama_clicked();
    void on_button_fog_clicked();
    void on_button_battleback_clicked();

    void set_fog_options(int fog_sx, int fog_sy, int fog_opacity, int fog_zoom, int fog_blend_type)
    {this->fog_sx = fog_sx; this->fog_sy = fog_sy; this->fog_opacity = fog_opacity; this->fog_zoom = fog_zoom; this->fog_blend_type = fog_blend_type; }



    void on_button_passage_clicked();
    void on_button_passage_4dir_clicked();
    void on_button_priority_clicked();
    void on_button_bush_flag_clicked();
    void on_button_counter_flag_clicked();
    void on_button_terrain_flag_clicked();

private:
    Ui::EditTilesets *ui;
    RPGEditorController *ec;
    int current;

    //int panorama_hue = 0;

    int fog_sx = 0;
    int fog_sy = 0;
    //int fog_hue = 0;
    int fog_opacity = 0;
    int fog_zoom = 0;
    int fog_blend_type = 0;

};

#endif // EDITTILESETS_H
