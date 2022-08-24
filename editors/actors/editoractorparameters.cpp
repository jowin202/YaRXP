#include "editoractorparameters.h"
#include "ui_editoractorparameters.h"

#include "editactors.h"

EditorActorParameters::EditorActorParameters(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorActorParameters)
{
    ui->setupUi(this);
}

EditorActorParameters::~EditorActorParameters()
{
    delete ui;
}

void EditorActorParameters::set_parent(EditActors *parent)
{
    this->parent = parent;
    for (int i = 0; i < 100; i++)
    {
        this->maxhp[i] = parent->maxhp[i];
        this->maxsp[i] = parent->maxsp[i];
        this->str[i] = parent->str[i];
        this->dex[i] = parent->dex[i];
        this->agi[i] = parent->agi[i];
        this->int_var[i] = parent->int_var[i];
    }

    this->ui->page_maxhp->set_parent_and_param(EditActors::MAXHP, this->maxhp, parent);
    this->ui->page_maxsp->set_parent_and_param(EditActors::MAXSP, this->maxsp, parent);
    this->ui->page_str->set_parent_and_param(EditActors::STR, this->str, parent);
    this->ui->page_dex->set_parent_and_param(EditActors::DEX, this->dex, parent);
    this->ui->page_agi->set_parent_and_param(EditActors::AGI, this->agi, parent);
    this->ui->page_int->set_parent_and_param(EditActors::INTVAR, this->int_var, parent);
}

void EditorActorParameters::set_page(int page)
{
    this->ui->tabWidget->setCurrentIndex(page);
}


void EditorActorParameters::on_button_cancel_clicked()
{
    this->close();
}

void EditorActorParameters::on_button_ok_clicked()
{
    for (int i = 0; i < 100; i++)
    {
        parent->maxhp[i] = this->maxhp[i];
        parent->maxsp[i] = this->maxsp[i];
        parent->str[i] = this->str[i];
        parent->dex[i] = this->dex[i];
        parent->agi[i] = this->agi[i];
        parent->int_var[i] = this->int_var[i];
    }
    emit params_set();
    this->close();
}
