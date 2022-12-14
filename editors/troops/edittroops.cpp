#include "edittroops.h"
#include "ui_edittroops.h"

#include "troopeventpage.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "RXIO2/factory.h"


#include "dialogs/imagedialog.h"

EditTroops::EditTroops(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditTroops)
{
    ui->setupUi(this);
}

EditTroops::~EditTroops()
{
    delete ui;
}

void EditTroops::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ui->label_troop_pic->setEC(ec);

    this->ec->connect_string_to_text_field(RPGDB::TROOPS, "@name", this->ui->line_name);
    connect(this->ec, &RPGEditorController::current_troop_changed, this, [=](){
        this->ec->fill_list(this->ui->list_enemies, RPGDB::ENEMIES, true, 3, false);

        QJsonArray page_array = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray();
        this->ui->tab_pages->clear();
        for (int i = 0; i < page_array.count(); i++)
        {
            TroopEventPage *page = new TroopEventPage;
            page->setTroopPage(ec,i);
            this->ui->tab_pages->addTab(page, QString::number(i+1));
        }

    });

}


void EditTroops::on_button_arrange_clicked()
{
    //this->ui->label_troop_pic->arrange();
    //TODO Do it here
    this->ui->label_troop_pic->redraw();
}

void EditTroops::on_button_clear_clicked()
{
    this->ec->obj_set_jsonvalue(RPGDB::TROOPS, "@members", QJsonArray());
    this->ui->label_troop_pic->redraw();
    this->ui->button_add->setEnabled(true); //always true after clear
}

void EditTroops::on_button_remove_clicked()
{
    this->ui->label_troop_pic->remove_current();
    QJsonArray members = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray();
    this->ui->button_add->setEnabled(true); //always true after remove
    this->ui->button_remove->setEnabled(members.count() > 0);
}

void EditTroops::on_button_add_clicked()
{
    int enemy_id = this->ui->list_enemies->currentRow()+1;
    QJsonArray members = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray();
    members.append(Factory().create_troop_member(enemy_id, false, false, 320, 304));
    this->ec->obj_set_jsonvalue(RPGDB::TROOPS, "@members", members);
    this->on_button_arrange_clicked(); //redraw should be done there
    this->ui->button_add->setEnabled(members.count() < 8);
}



void EditTroops::on_button_autoname_clicked()
{
    QStringList name_list;
    QStringList counted_name_list;
    QJsonArray members = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray();
    for (int i = 0; i < members.count(); i++)
    {
        QJsonObject enemy = ec->get_object_by_id(RPGDB::ENEMIES, members.at(i).toObject().value("@enemy_id").toInt());
        name_list << enemy.value("@name").toString();
    }

    QString result = "";
    for (int i = 0; i < name_list.length(); i++)
    {
        QString current = name_list.first();
        int cnt = 0;
        for (int j = 0; j < name_list.length(); j++)
        {
            if (current == name_list.at(j))
            {
                cnt++;
                name_list.removeAt(j);
                j--;
            }
        }
        if (cnt > 1)
            current = current + "*" + QString::number(cnt);

        counted_name_list.append(current);

    }
    this->ui->line_name->setText(counted_name_list.join(", "));
}


void EditTroops::on_button_edit_battleback_clicked()
{
    QString current_battleback_name = ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battleback_name").toString();
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::BATTLEBACKS, current_battleback_name, nullptr);
    connect(dialog, SIGNAL(ok_clicked(QString)), this, SLOT(set_new_battleback(QString)));
    dialog->show();

}

void EditTroops::set_new_battleback(QString battleback_name)
{
    ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@battleback_name", battleback_name);
    this->ui->label_troop_pic->redraw();
}


void EditTroops::on_button_delete_page_clicked()
{
    QJsonArray page_array = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray();
    page_array.removeAt(this->ui->tab_pages->currentIndex());
    ec->obj_set_jsonvalue(RPGDB::TROOPS, "@pages", page_array);
    ec->refresh(RPGDB::TROOPS);
}


void EditTroops::on_button_copy_page_clicked()
{
    int p = this->ui->tab_pages->currentIndex();
    QJsonObject page = this->ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray().at(p).toObject();
    QJsonDocument doc(page);

    QSettings settings;
    settings.setValue("copied_troop_page", doc.toJson(QJsonDocument::Compact));
}


void EditTroops::on_button_paste_page_clicked()
{
    int p = this->ui->tab_pages->currentIndex();

    QSettings settings;
    QByteArray json = settings.value("copied_troop_page").toByteArray();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
    if (err.error != QJsonParseError::NoError) return;


    QJsonArray pages = this->ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray();
    pages.insert(p+1,doc.object()); //insert after
    this->ec->obj_set_jsonvalue(RPGDB::TROOPS, "@pages", pages);

    ec->refresh(RPGDB::TROOPS);
    this->ui->tab_pages->setCurrentIndex(p);
}

