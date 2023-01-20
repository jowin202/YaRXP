#ifndef EVENTLISTITEM_H
#define EVENTLISTITEM_H

#include <QListWidgetItem>
#include <QObject>
#include <QWidget>
#include <QInputDialog>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class RPGDB;
class RPGMapController;
class RPGMapInfoController;

class EventListItem : public QListWidgetItem
{
public:
    EventListItem(QListWidget *item, RPGMapController *mc, RPGMapInfoController *mic, QJsonObject obj);
    QString get_text(QJsonObject obj);
    void setColor(int code);

    void update_text();
    QJsonObject get_obj() { return this->obj; }

    void edit_cell();

public slots:
    void set_parameters(QJsonArray parameters) {this->obj.insert("@parameters", parameters); this->update_text(); }

private:
    QColor black = QColor(0,0,0);
    QColor green = QColor(0x24, 0x8d, 0x23);
    QColor dark_red = QColor(0x97, 0x07, 0x0b);
    QColor red = QColor(0xff, 0x00, 0x00);
    QColor grey = QColor(0x9a, 0x9a, 0x9a);
    QColor blue = QColor(0, 0, 0xFF);
    QColor pink = QColor(0xFF, 0, 0xFF);
    QColor dark_pink = QColor(150, 0, 150);
    QColor light_green = QColor(150,150,0);
    QColor light_blue = QColor(0,150,150);
    QColor orange = QColor(255,150,0);
    QColor cyan = QColor(0x1e,0x90,0xff);


    QStringList text_option_pos = { "Top", "Middle", "Bottom" };
    QStringList text_option_win = { "Show", "Hide" };
    QStringList text_on_off = {"ON", "OFF" };
    QStringList text_compare_op = { "==", " >=", "<=", ">", "<" , "!=" };
    QStringList text_directions = { "Down", "Left", "Right", "Up" };
    QStringList text_buttons = { "A", "B", "C", "X", "Y", "Z", "L", "R" };
    QStringList text_var_operators = { "=", "+=", "-=", "*=", "/=", "%=" };
    QStringList text_stat_vars = { "Level", "EXP", "HP", "SP", "MaxHP", "MaxSP", "STR", "DEX", "AGI", "INT", "ATK", "PDEF", "MDEF", "EVA"};
    QStringList text_map_vars = { "Map X", "Map Y", "Direction", "Screen X", "Screen Y", "Terrain Tag" };
    QStringList text_other_vars = { "Map ID", "Party Members", "Gold", "Steps", "Play Time", "Timer", "Save Count" };
    QStringList text_pm = {"+", "-" };
    QStringList text_en_dis = { "Disable", "Enable" };
    QStringList text_blend { "Normal", "Add", "Sub" };

    QStringList text_move_routes = { "Move Down", "Move Left", "Move Right", "Move Up",
                                     "Move Lower Left", "Move Lower Right", "Move Upper Left", "Move Upper Right",
                                   "Move at Random", "Move toward Player", "Move away from Player", "1 Step Forward", "1 Step Backward",
                                   "Jump: ", "Wait: ",

                                   "Turn Down", "Turn Left", "Turn Right", "Turn Up", "Turn 90° Right", "Turn 90° Left", "Turn 180°",
                                   "Turn 90° Right or Left", "Turn at Random", "Turn toward Player", "Turn away from Player",
                                   "Switch ON: ", "Switch OFF: ", "Change Speed: ", "Change Freq: ",

                                   "Move Animation ON", "Move Animation OFF", "Stop Animation ON", "Stop Animation OFF", "Direction Fix ON", "Direction Fix OFF",
                                   "Through ON", "Through OFF", "Always on Top ON", "Always on Top OFF", "Graphic: ",
                                   "Change Opacity: ", "Change Blending: ", "SE: ", "Script: " };

    QStringList text_weather = { "None", "Rain", "Storm", "Snow" };
    QStringList text_basic_actions = { "Attack", "Defend", "Escape", "Do Nothing" };


    QJsonObject obj;
    RPGDB *db;
    RPGMapController *mc;
    RPGMapInfoController *mic;
    QListWidget*parent;
};

#endif // EVENTLISTITEM_H
