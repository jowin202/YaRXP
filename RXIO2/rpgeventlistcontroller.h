#ifndef RPGEVENTLISTCONTROLLER_H
#define RPGEVENTLISTCONTROLLER_H

#include <QObject>
#include <QListWidget>

class RPGDB;
class RPGMapController;

class RPGEventListController : public QObject
{
    Q_OBJECT
public:
    RPGEventListController(RPGMapController *mc, QJsonArray list, QListWidget *listwidget);


    void setColor(int code, QListWidgetItem *item)
    {
        if ((code >= 351 && code <= 355) || code == 655) item->setForeground(grey);
        else if (code == 209 || code == 509) item->setForeground(dark_red);
        else if (code == 108 || code == 408) item->setForeground(green);
        else if ((code >= 112 && code <= 119) || code == 413) item->setForeground(blue);
        else if ((code >= 121 && code <= 129)) item->setForeground(red);
        else if ((code >= 131 && code <= 136)) item->setForeground(pink);
        else if (code >= 201 && code <= 208) item->setForeground(dark_red);
        else if (code >= 221 && code <= 225) item->setForeground(light_green);
        else if (code >= 231 && code <= 236) item->setForeground(dark_pink);
        else if (code >= 241 && code <= 251) item->setForeground(light_blue);
        else if ((code >= 301 && code <= 303) || (code >= 601 && code <= 605)) item->setForeground(orange);
        else if (code >= 311 && code <= 322) item->setForeground(cyan);
        else if (code >= 331 && code <= 340) item->setForeground(dark_pink); //TODO: should be slightly different to 231-236
        else item->setForeground(black);
    }


    QString get_text(QJsonObject obj);

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

    RPGDB *db;
    RPGMapController *mc;

    QStringList text_option_pos = { "Top", "Middle", "High" };
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
    QStringList text_en_dis { "Disable", "Enable" };
signals:


};

#endif // RPGEVENTLISTCONTROLLER_H
