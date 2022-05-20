#ifndef MAPPROPERTIES_H
#define MAPPROPERTIES_H

#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

#include "parser/rpgmap.h"
#include "parser/rpgmapinfo.h"
#include "settings.h"

namespace Ui {
class MapProperties;
}

class MapProperties : public QWidget
{
    Q_OBJECT

public:
    explicit MapProperties(RPGMapInfo* info, Settings *settings, QWidget *parent = nullptr);
    ~MapProperties();
    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Return)
            this->on_button_ok_clicked();
        if (event->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
    }
private slots:
    void on_check_auto_change_bgm_toggled(bool checked);
    void on_check_auto_change_bgs_toggled(bool checked);


    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::MapProperties *ui;
    RPGMapInfo *mapinfo;
};

#endif // MAPPROPERTIES_H
