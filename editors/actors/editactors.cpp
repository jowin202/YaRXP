                #include "editactors.h"
#include "ui_editactors.h"

#include "dialogs/imagedialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

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

void EditActors::setEC(RPGEditorController *ec)
{
    this->ec = ec;

    this->ec->connect_string_to_text_field(RPGDB::ACTORS, "@name", this->ui->line_name);
    this->ec->connect_int_to_spin_box(RPGDB::ACTORS, "@initial_level", this->ui->spin_initial);
    this->ec->connect_int_to_spin_box(RPGDB::ACTORS, "@final_level", this->ui->spin_final);

    connect(this->ec, SIGNAL(current_actor_changed()), this, SLOT(set_exp_curve()));
    connect(this->ec, SIGNAL(current_actor_changed()), this, SLOT(import_params()));

    this->ec->connect_int_to_data_combo_box(RPGDB::ACTORS, "@class_id", this->ui->combo_class, RPGDB::CLASSES, true, 3, false);

    //refresh custom combos
    connect(this->ui->combo_class, &QComboBox::currentIndexChanged, ec, [=]() {
        if (!ec->block_writing) //only refresh when not reloading page otherwise infinite loop
        {
            ec->refresh(RPGDB::ACTORS);
            this->ec->obj_set_jsonvalue(RPGDB::ACTORS, "@weapon_id", this->ui->combo_weapon->currentData().toInt());
            this->ec->obj_set_jsonvalue(RPGDB::ACTORS, "@armor1_id", this->ui->combo_shield->currentData().toInt());
            this->ec->obj_set_jsonvalue(RPGDB::ACTORS, "@armor2_id", this->ui->combo_helmet->currentData().toInt());
            this->ec->obj_set_jsonvalue(RPGDB::ACTORS, "@armor3_id", this->ui->combo_body->currentData().toInt());
            this->ec->obj_set_jsonvalue(RPGDB::ACTORS, "@armor4_id", this->ui->combo_accessory->currentData().toInt());
        }
    } );

    this->ec->connect_int_to_data_combo_box(RPGDB::ACTORS, "@weapon_id", this->ui->combo_weapon, RPGDB::CLASSES_WEAPONS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ACTORS, "@armor1_id", this->ui->combo_shield, RPGDB::CLASSES_SHIELD, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ACTORS, "@armor2_id", this->ui->combo_helmet, RPGDB::CLASSES_HELMET, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ACTORS, "@armor3_id", this->ui->combo_body, RPGDB::CLASSES_BODY, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ACTORS, "@armor4_id", this->ui->combo_accessory, RPGDB::CLASSES_ACCESSORY, true, 3, true);

    this->ec->connect_bool_to_check_box(RPGDB::ACTORS, "@weapon_fix", this->ui->check_weapon);
    this->ec->connect_bool_to_check_box(RPGDB::ACTORS, "@armor1_fix", this->ui->check_shield);
    this->ec->connect_bool_to_check_box(RPGDB::ACTORS, "@armor2_fix", this->ui->check_helmet);
    this->ec->connect_bool_to_check_box(RPGDB::ACTORS, "@armor3_fix", this->ui->check_body);
    this->ec->connect_bool_to_check_box(RPGDB::ACTORS, "@armor4_fix", this->ui->check_accessory);

    this->ec->connect_image_display_widget(RPGDB::ACTORS, ImageDialog::CHARACTERS, "@character_name", "@character_hue", this->ui->label_character_graphic);
    this->ec->connect_image_display_widget(RPGDB::ACTORS, ImageDialog::BATTLERS, "@battler_name", "@battler_hue", this->ui->label_battler_graphic);

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



void EditActors::set_exp_curve()
{
    this->exp_basis = this->ec->obj_get_jsonvalue(RPGDB::ACTORS, "@exp_basis").toInt();
    this->exp_inflation = this->ec->obj_get_jsonvalue(RPGDB::ACTORS, "@exp_inflation").toInt();
    this->ui->line_exp_curve->setText(QString("Basis: %1, Inflation: %2").arg(this->exp_basis).arg(this->exp_inflation));
}

void EditActors::import_params()
{
    QJsonArray arr = this->ec->obj_get_jsonvalue(RPGDB::ACTORS, "@parameters").toObject().value("values").toArray();

    for (int i = 0; i < arr.size(); i++)
    {
        if (i%6 == 0)
            this->maxhp[i/6] = arr.at(i).toInt();
        else if (i%6 == 1)
            this->maxsp[i/6] = arr.at(i).toInt();
        else if (i%6 == 2)
            this->str[i/6] = arr.at(i).toInt();
        else if (i%6 == 3)
            this->dex[i/6] = arr.at(i).toInt();
        else if (i%6 == 4)
            this->agi[i/6] = arr.at(i).toInt();
        else if (i%6 == 5)
            this->int_var[i/6] = arr.at(i).toInt();
    }
    this->update_params();
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

void EditActors::export_params()
{
    QJsonArray arr;

    for (int i = 0; i < 600; i++)
    {
        if (i%6 == 0)
            arr.append(QJsonValue(this->maxhp[i/6]));
        else if (i%6 == 1)
            arr.append(QJsonValue(this->maxsp[i/6]));
        else if (i%6 == 2)
            arr.append(QJsonValue(this->str[i/6]));
        else if (i%6 == 3)
            arr.append(QJsonValue(this->dex[i/6]));
        else if (i%6 == 4)
            arr.append(QJsonValue(this->agi[i/6]));
        else if (i%6 == 5)
            arr.append(QJsonValue(this->int_var[i/6]));
    }

    this->ec->current_actor_set_parameters(arr);
}


void EditActors::on_button_exp_curve_clicked()
{
    EditExpCurve *dialog = new EditExpCurve;
    dialog->set_values(this->exp_basis,this->exp_inflation);
    connect(dialog, &EditExpCurve::exp_curve_set, this->ec, [=](int b, int i)
    {
        this->ec->obj_set_jsonvalue(RPGDB::ACTORS, "@exp_basis", b);
        this->ec->obj_set_jsonvalue(RPGDB::ACTORS, "@exp_inflation", i);
    });
    connect(dialog, SIGNAL(exp_curve_set(int,int)), this, SLOT(set_exp_curve()));
    dialog->show();

}

void EditActors::on_button_param_maxhp_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(MAXHP);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    connect(dialog, SIGNAL(params_set()), this, SLOT(export_params()));
    dialog->show();
}

void EditActors::on_button_param_maxsp_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(MAXSP);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    connect(dialog, SIGNAL(params_set()), this, SLOT(export_params()));
    dialog->show();
}

void EditActors::on_button_param_str_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(STR);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    connect(dialog, SIGNAL(params_set()), this, SLOT(export_params()));
    dialog->show();

}

void EditActors::on_button_param_dex_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(DEX);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    connect(dialog, SIGNAL(params_set()), this, SLOT(export_params()));
    dialog->show();

}

void EditActors::on_button_param_agi_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(AGI);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    connect(dialog, SIGNAL(params_set()), this, SLOT(export_params()));
    dialog->show();

}

void EditActors::on_button_param_int_clicked()
{
    EditorActorParameters *dialog = new EditorActorParameters;
    dialog->set_parent(this);
    dialog->set_page(INTVAR);
    connect(dialog, SIGNAL(params_set()), this, SLOT(update_params()));
    connect(dialog, SIGNAL(params_set()), this, SLOT(export_params()));
    dialog->show();
}
