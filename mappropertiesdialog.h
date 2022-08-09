#ifndef MAPPROPERTIESDIALOG_H
#define MAPPROPERTIESDIALOG_H

#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

#include "RXIO/RXObjects/rpgmap.h"
#include "RXIO/RXObjects/rpgmapinfo.h"
#include "RXIO/RXObjects/rpgsettings.h"

namespace Ui {
class MapPropertiesDialog;
}

class MapPropertiesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MapPropertiesDialog(RPGMapInfo* info, RPGSettings *settings, QWidget *parent = nullptr);
    ~MapPropertiesDialog();
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
    Ui::MapPropertiesDialog *ui;
    RPGMapInfo *mapinfo;
};

#endif // MAPPROPERTIESDIALOG_H
