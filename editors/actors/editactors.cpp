#include "editactors.h"
#include "ui_editactors.h"

#include "dialogs/imagedialog.h"

#include "editexpcurve.h"
#include "editoractorparameters.h"

EditActors::EditActors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditActors)
{
    ui->setupUi(this);
}

EditActors::~EditActors()
{
    delete ui;
}

QImage EditActors::get_image_from_param(int param, int *values, bool big)
{
    QColor col;
    int height, width;
    int val_width;
    qreal height_factor;
    qreal max_val = 999.0;

    if (param == MAXHP)
    {
        col.setRgb(0xc8,0x3c,0x78);
        max_val = 9999.0;
    }
    else if (param == MAXSP)
    {
        col.setRgb(0x3c,0x78,0xc8);
        max_val = 9999.0;
    }
    else if (param == STR)
    {
        col.setRgb(0xc8,0x78,0x3c);
    }
    else if (param == DEX)
    {
        col.setRgb(0x78,0xc8,0x3c);
    }
    else if (param == AGI)
    {
        col.setRgb(0x3c,0xc8,0x78);
    }
    else //if (param == INTVAR)
    {
        col.setRgb(0x78,0x3c,0xc8);
    }


    if (big) {
        width = 400;
        height = 250;
        val_width = 4;
        height_factor = 250.0/max_val;
    }
    else {
        width = 200;
        height = 100;
        val_width = 2;
        height_factor = 100.0/max_val;
    }

    QImage img(width,height, QImage::Format_RGB32); //non transparent
    QPainter painter;
    painter.begin(&img);
    painter.fillRect(0,0,width,height,Qt::white);
    for (int i = 1; i < 100; i++)
    {
        int h = height_factor * values[i];
        painter.fillRect(val_width*i, height - h, val_width, h, col);
    }
    painter.end();
    return img;
}



void EditActors::set_exp_curve(int basis, int inflation)
{
    this->exp_basis = basis;
    this->exp_inflation = inflation;
    this->ui->line_exp_curve->setText(QString("Basis: %1, Inflation: %2").arg(basis).arg(inflation));
}

void EditActors::update_params()
{
    this->ui->button_param_maxhp->setIcon(QPixmap::fromImage(this->get_image_from_param(MAXHP, this->maxhp, false)));
    this->ui->button_param_maxsp->setIcon(QPixmap::fromImage(this->get_image_from_param(MAXSP, this->maxsp,false)));
    this->ui->button_param_str->setIcon(QPixmap::fromImage(this->get_image_from_param(STR, this->str,false)));
    this->ui->button_param_dex->setIcon(QPixmap::fromImage(this->get_image_from_param(DEX, this->dex,false)));
    this->ui->button_param_agi->setIcon(QPixmap::fromImage(this->get_image_from_param(AGI, this->agi,false)));
    this->ui->button_param_int->setIcon(QPixmap::fromImage(this->get_image_from_param(INTVAR, this->int_var, false)));
}


void EditActors::on_button_exp_curve_clicked()
{
    EditExpCurve *dialog = new EditExpCurve;
    dialog->set_values(this->exp_basis,this->exp_inflation);
    connect(dialog,SIGNAL(exp_curve_set(int,int)),this, SLOT(set_exp_curve(int,int)));
    dialog->show();

}

void EditActors::set_actor(int n)
{
    if (this->system->actor_list.length() <= n) return;
    RPGActor *current_actor = this->system->actor_list.at(n);
    this->current = n;

    this->ui->line_name->setText(current_actor->name);
    this->ui->spin_initial->setValue(current_actor->initial_level);
    this->ui->spin_final->setValue(current_actor->final_level);

    system->datasource.fill_combo(this->ui->combo_class,RPGSystem::CLASSES,true, 3, current_actor->class_id, false);

    system->datasource.fill_combo_weapon_by_class(this->ui->combo_weapon, current_actor->class_id, true, 3, current_actor->weapon_id);
    system->datasource.fill_combo_shield_by_class(this->ui->combo_shield, current_actor->class_id, true, 3, current_actor->armor1_id);
    system->datasource.fill_combo_helmet_by_class(this->ui->combo_helmet, current_actor->class_id, true, 3, current_actor->armor2_id);
    system->datasource.fill_combo_body_by_class(this->ui->combo_body, current_actor->class_id, true, 3, current_actor->armor3_id);
    system->datasource.fill_combo_accessory_by_class(this->ui->combo_accessory, current_actor->class_id, true, 3, current_actor->armor4_id);

    this->set_exp_curve(current_actor->exp_basis, current_actor->exp_inflation);


    this->ui->label_character_graphic->set_data(system, ImageDialog::CHARACTERS, current_actor->character_name, current_actor->character_hue);
    this->ui->label_battler_graphic->set_data(system, ImageDialog::BATTLERS, current_actor->battler_name, current_actor->battler_hue);

    //copy params from actor
    for (int i = 0; i < 100; i++) //TODO: check if start from 1 or 0
    {
        this->maxhp[i] = current_actor->maxhp.at(i);
        this->maxsp[i] = current_actor->maxsp.at(i);
        this->str[i] = current_actor->str.at(i);
        this->dex[i] = current_actor->dex.at(i);
        this->agi[i] = current_actor->agi.at(i);
        this->int_var[i] = current_actor->int_var.at(i);
    }
    this->update_params();

    this->ui->check_weapon->setChecked(current_actor->weapon_fix);
    this->ui->check_shield->setChecked(current_actor->armor1_fix);
    this->ui->check_helmet->setChecked(current_actor->armor2_fix);
    this->ui->check_body->setChecked(current_actor->armor3_fix);
    this->ui->check_accessory->setChecked(current_actor->armor4_fix);

    /*
    qDebug() << current_actor->to_hash();
    QFile test("/tmp/opened.json");
    test.open(QIODevice::WriteOnly);
    test.write(current_actor->to_json());
    test.close();
    */
}

void EditActors::save()
{
    int n = this->current;
    if (this->system->actor_list.length() <= n) return;
    RPGActor *current_actor = this->system->actor_list.at(n);


    current_actor->name = this->ui->line_name->text();
    current_actor->initial_level = this->ui->spin_initial->value();
    current_actor->final_level = this->ui->spin_final->value();

    current_actor->class_id = this->ui->combo_class->currentData().toInt();

    current_actor->weapon_id = this->ui->combo_weapon->currentData().toInt();
    current_actor->armor1_id = this->ui->combo_shield->currentData().toInt();
    current_actor->armor2_id = this->ui->combo_helmet->currentData().toInt();
    current_actor->armor3_id = this->ui->combo_body->currentData().toInt();
    current_actor->armor4_id = this->ui->combo_accessory->currentData().toInt();

    current_actor->exp_basis = this->exp_basis;
    current_actor->exp_inflation = this->exp_inflation;

    current_actor->character_name = this->ui->label_character_graphic->current_file;
    current_actor->character_hue = this->ui->label_character_graphic->hue;

    current_actor->battler_name = this->ui->label_battler_graphic->current_file;
    current_actor->battler_hue = this->ui->label_battler_graphic->hue;

    current_actor->maxhp.clear();
    current_actor->maxsp.clear();
    current_actor->str.clear();
    current_actor->dex.clear();
    current_actor->agi.clear();
    current_actor->int_var.clear();
    for (int i = 0; i < 100; i++)
    {
        current_actor->maxhp.append(this->maxhp[i]);
        current_actor->maxsp.append(this->maxsp[i]);
        current_actor->str.append(this->str[i]);
        current_actor->dex.append(this->dex[i]);
        current_actor->agi.append(this->agi[i]);
        current_actor->int_var.append(this->int_var[i]);
    }

    current_actor->weapon_fix = this->ui->check_weapon->isChecked();
    current_actor->armor1_fix = this->ui->check_shield->isChecked();
    current_actor->armor2_fix = this->ui->check_helmet->isChecked();
    current_actor->armor3_fix = this->ui->check_body->isChecked();
    current_actor->armor4_fix = this->ui->check_accessory->isChecked();

    /*
    qDebug() << current_actor->to_hash();
    QFile test("/tmp/saved.json");
    test.open(QIODevice::WriteOnly);
    test.write(current_actor->to_json());
    test.close();
    */
}


void EditActors::on_button_param_maxhp_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(MAXHP);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    dialog->show();
}

void EditActors::on_button_param_maxsp_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(MAXSP);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    dialog->show();
}

void EditActors::on_button_param_str_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(STR);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    dialog->show();

}

void EditActors::on_button_param_dex_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(DEX);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    dialog->show();

}

void EditActors::on_button_param_agi_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(AGI);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    dialog->show();

}

void EditActors::on_button_param_int_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(INTVAR);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    dialog->show();
}
