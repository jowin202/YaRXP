#include "editoractorparameterpage.h"
#include "ui_editoractorparameterpage.h"

#include "editactors.h"


EditorActorParameterPage::EditorActorParameterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorActorParameterPage)
{
    ui->setupUi(this);
    connect(this->ui->label_curve, SIGNAL(curve_changed(int,int)), this, SLOT(update_curve(int,int)));
    connect(this->ui->spin_level, SIGNAL(valueChanged(int)), this, SLOT(update_curve_from_spin()));
    connect(this->ui->spin_value, SIGNAL(valueChanged(int)), this, SLOT(update_curve_from_spin()));
    rand.seed(QDateTime::currentMSecsSinceEpoch());
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
    int start, end;
    if (param == EditActors::MAXHP || param == EditActors::MAXSP)
    {
        start = rand.bounded(700,799);
        end = rand.bounded(7000,7999);
    }
    else
    {
        start = rand.bounded(70,79);
        end = rand.bounded(700,799);
    }
    this->set_linear(start,end);
}

void EditorActorParameterPage::on_button_B_clicked()
{
    int start, end;
    if (param == EditActors::MAXHP || param == EditActors::MAXSP)
    {
        start = rand.bounded(600,699);
        end = rand.bounded(6000,6999);
    }
    else
    {
        start = rand.bounded(60,69);
        end = rand.bounded(600,699);
    }
    this->set_linear(start,end);
}

void EditorActorParameterPage::on_button_C_clicked()
{
    int start, end;
    if (param == EditActors::MAXHP || param == EditActors::MAXSP)
    {
        start = rand.bounded(500,599);
        end = rand.bounded(5000,5999);
    }
    else
    {
        start = rand.bounded(50,59);
        end = rand.bounded(500,599);
    }
    this->set_linear(start,end);
}

void EditorActorParameterPage::on_button_D_clicked()
{
    int start, end;
    if (param == EditActors::MAXHP || param == EditActors::MAXSP)
    {
        start = rand.bounded(400,499);
        end = rand.bounded(4000,4999);
    }
    else
    {
        start = rand.bounded(40,49);
        end = rand.bounded(400,499);
    }
    this->set_linear(start,end);
}

void EditorActorParameterPage::on_button_E_clicked()
{
    int start, end;
    if (param == EditActors::MAXHP || param == EditActors::MAXSP)
    {
        start = rand.bounded(300,399);
        end = rand.bounded(3000,3999);
    }
    else
    {
        start = rand.bounded(30,39);
        end = rand.bounded(300,399);
    }
    this->set_linear(start,end);
}

void EditorActorParameterPage::update_curve(int level, int value)
{
    this->ui->spin_level->setValue(level);
    this->ui->spin_value->setValue(value);
    this->ui->label_curve->setPixmap(QPixmap::fromImage(parent->get_image_from_param(param,values,true)));
}

void EditorActorParameterPage::update_curve_from_spin()
{
    values[this->ui->spin_level->value()] = this->ui->spin_value->value();
    this->ui->label_curve->setPixmap(QPixmap::fromImage(parent->get_image_from_param(param,values,true)));
}

void EditorActorParameterPage::set_linear(int start, int end)
{
    values[1] = start;
    values[99] = end;

    for (int i = 2; i <= 98; i++)
        values[i] = start + 1.0*(end - start)/98.0 * (i-1);


    this->ui->label_curve->setPixmap(QPixmap::fromImage(parent->get_image_from_param(param,values,true)));
}

