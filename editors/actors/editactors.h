#ifndef EDITACTORS_H
#define EDITACTORS_H

#include <QWidget>
#include "../../RXIO/RXObjects/rpgactor.h"
#include "../../RXIO/RXObjects/rpgsystem.h"

namespace Ui {
class EditActors;
}

class EditActors : public QWidget
{
    Q_OBJECT

public:
    explicit EditActors(QWidget *parent = nullptr);
    ~EditActors();

    void setSystem(RPGSystem *system)
    {this->system = system;}

    enum {MAXHP, MAXSP, STR, DEX, AGI, INTVAR};
    QImage get_image_from_param(int param, int *values, bool big);


    //Variables from another dialog, dont forget them when saving
    int exp_basis;
    int exp_inflation;

    int maxhp[100];
    int maxsp[100];
    int str[100];
    int dex[100];
    int agi[100];
    int int_var[100];



public slots:
    void set_exp_curve(int basis, int inflation);
    void update_params();
    void on_button_exp_curve_clicked();
    void set_actor(int n);

private slots:


    void on_button_param_maxhp_clicked();
    void on_button_param_maxsp_clicked();
    void on_button_param_str_clicked();
    void on_button_param_dex_clicked();
    void on_button_param_agi_clicked();
    void on_button_param_int_clicked();


private:
    RPGSystem *system;
    Ui::EditActors *ui;
};

#endif // EDITACTORS_H
