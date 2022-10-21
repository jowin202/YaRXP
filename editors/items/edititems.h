#ifndef EDITITEMS_H
#define EDITITEMS_H

#include <QWidget>

#include "RXIO/RXObjects/rpgaudiofile.h"

namespace Ui {
class EditItems;
}

class RPGSystem;

class EditItems : public QWidget
{
    Q_OBJECT

public:
    explicit EditItems(QWidget *parent = nullptr);
    ~EditItems();

    void setSystem(RPGSystem *system)
    {this->system = system;}

    void set_item(int n);
    void save();


private slots:
    void on_combo_parameter_currentIndexChanged(int index);


    void on_button_se_clicked();

    void on_button_icon_clicked();

private:
    Ui::EditItems *ui;
    RPGSystem *system;
    RPGAudioFile menu_se;

    int current;
};

#endif // EDITITEMS_H
