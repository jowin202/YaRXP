#include "editoractorparameterpage.h"
#include "ui_editoractorparameterpage.h"

#include "editactors.h"

EditorActorParameterPage::EditorActorParameterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorActorParameterPage)
{
    ui->setupUi(this);
    connect(this->ui->label_curve, SIGNAL(curve_changed(int,int)), this, SLOT(update_curve(int,int)));
}


EditorActorParameterPage::~EditorActorParameterPage()
{
    delete ui;
}

void EditorActorParameterPage::set_parent_and_param(int param, int *values, EditActors *parent)
{
    this->parent = parent;
    this->values = values;
    this->param = param;

    if (param == EditActors::MAXHP || param == EditActors::MAXSP)
        this->ui->label_curve->set_values(values, 9999.0);
    else
        this->ui->label_curve->set_values(values, 999.0);

    this->update_curve(1, values[1]);
}

void EditorActorParameterPage::on_button_A_clicked()
{

}

void EditorActorParameterPage::on_button_B_clicked()
{

}

void EditorActorParameterPage::on_button_C_clicked()
{

}

void EditorActorParameterPage::on_button_D_clicked()
{

}

void EditorActorParameterPage::on_button_E_clicked()
{

}

void EditorActorParameterPage::update_curve(int level, int value)
{
    this->ui->spin_level->setValue(level);
    this->ui->spin_value->setValue(value);
    this->ui->label_curve->setPixmap(QPixmap::fromImage(parent->get_image_from_param(param,values,true)));
}
