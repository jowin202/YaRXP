#include "mapencounterdialog.h"
#include "ui_mapencounterdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/rpgmapcontroller.h"

#include "RXIO2/writer.h"
#include "RXIO2/parser.h"
#include "RXIO2/fileopener.h"

#include "../pbsfactory.h"
#include "encounterwidget.h"

MapEncounterDialog::MapEncounterDialog(RPGDB *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapEncounterDialog)
{
    ui->setupUi(this);
    this->db = db;


    QString filepath_encounters = FileOpener(db->data_dir,"encounters.dat").get_existing_file();
    if (filepath_encounters.isEmpty())
    {
        QMessageBox::critical(this, "Not supported", "This feature is not supported in your project");
        this->close();
        return;
    }
    Parser parser(&encounters_file, filepath_encounters, false, true);

    this->list_maps();
    this->ui->treeWidget->hideColumn(1);
    this->ui->treeWidget->hideColumn(2);

    connect(this->ui->combo_version, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_version_changed()));
}

MapEncounterDialog::~MapEncounterDialog()
{
    delete ui;
}

void MapEncounterDialog::list_maps()
{
    RPGMapInfoController mic = RPGMapInfoController(db);
    QJsonArray toplevel_maps = mic.get_toplevel_maps();
    QJsonArray non_toplevel_maps = mic.get_child_maps();

    for (int i = 0; i < toplevel_maps.count(); i++)
    {
        QStringList columns;
        columns << toplevel_maps.at(i).toObject().value("@name").toString();
        columns << QString::number(toplevel_maps.at(i).toObject().value("@order").toInt()).rightJustified(3,'0');
        columns << QString::number(toplevel_maps.at(i).toObject().value("id").toInt());
        QTreeWidgetItem *item = new QTreeWidgetItem(columns);
        this->ui->treeWidget->addTopLevelItem(item);
        this->id_map.insert(toplevel_maps.at(i).toObject().value("id").toInt(), item);
        this->ui->treeWidget->expandItem(item);
    }


    while(non_toplevel_maps.count() > 0)
    {
        int cnt1 = non_toplevel_maps.count();
        for (int i = 0; i < non_toplevel_maps.count(); i++)
        {
            if (this->id_map.contains(non_toplevel_maps.at(i).toObject().value("@parent_id").toInt()))
            {
                QStringList columns;
                columns << non_toplevel_maps.at(i).toObject().value("@name").toString();
                columns << QString::number(non_toplevel_maps.at(i).toObject().value("@order").toInt()).rightJustified(3,'0');
                columns << QString::number(non_toplevel_maps.at(i).toObject().value("id").toInt());
                QTreeWidgetItem *item = new QTreeWidgetItem(columns);
                this->id_map.value(non_toplevel_maps.at(i).toObject().value("@parent_id").toInt())->addChild(item);
                this->id_map.insert(non_toplevel_maps.at(i).toObject().value("id").toInt(), item);
                this->ui->treeWidget->expandItem(item);
                non_toplevel_maps.removeAt(i--); //do this at the end

            }
        }
        int cnt2 = non_toplevel_maps.count();
        if (cnt1 == cnt2)
            break; //break here if parent_id loop
    }
    this->ui->treeWidget->sortItems(1,Qt::SortOrder::AscendingOrder);
}

void MapEncounterDialog::change_current(int version_index, int map, int version)
{
    QJsonObject file_obj = encounters_file.object();
    QJsonArray dict_list = file_obj.value("dict_list").toArray();

    PBSFactory factory;
    EncounterWidget *w;
    QJsonArray new_encounter = factory.create_encounter(map, version);
    for (int i = 0; i < this->ui->encounter_layout->count(); i++)
    {
        if (this->ui->encounter_layout->itemAt(i) == 0) continue;
        if ((w=dynamic_cast<EncounterWidget*>(this->ui->encounter_layout->itemAt(i)->widget())) != nullptr)
        {
            new_encounter = factory.encounter_add_type(new_encounter, w->type(), w->step_chance());
            new_encounter = w->add_slot_data(new_encounter);
        }
    }
    dict_list.removeAt(version_index);
    dict_list.insert(version_index, new_encounter);
    file_obj.insert("dict_list", dict_list);
    encounters_file.setObject(file_obj);
}

void MapEncounterDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous); //don't use this
    this->current_id = current->text(2).toInt();

    this->ui->combo_version->clear();
    QLayoutItem *item;
     while ( (item = this->ui->encounter_layout->takeAt(0)) != 0)
         delete item->widget();

    QJsonArray dict_list = encounters_file.object().value("dict_list").toArray();
    for (int i = 0; i < dict_list.count(); i++)
    {
        if (dict_list.at(i).toArray().at(1).toObject().value("@map").toInt() == current_id)
        {
            // Widget(map,id)
            int version = dict_list.at(i).toArray().at(1).toObject().value("@version").toInt();
            this->ui->combo_version->addItem(QString("Version %1").arg(version), i);
        }
    }
    this->ui->combo_version->setEnabled(this->ui->combo_version->count() > 0);
    this->ui->button_remove_version->setEnabled(this->ui->combo_version->count() > 0);
}

void MapEncounterDialog::combo_version_changed()
{
    bool ok;
    int version_index = this->ui->combo_version->currentData().toInt(&ok);
    if (!ok) return;

    //delete current
    QLayoutItem *item;
     while ( (item = this->ui->encounter_layout->takeAt(0)) != 0)
         delete item->widget();

     //flags for new created
     bool create_new = false;
     int version;

    QJsonArray encounters_data;
    if (version_index >= 0) //version already exists
        encounters_data = encounters_file.object().value("dict_list").toArray().at(version_index).toArray();
    else
    {
        //create this new
        create_new = true;
        PBSFactory factory;
        version = -version_index-1; //use this again for saving to file
        encounters_data = factory.create_encounter(this->current_id,version);
        encounters_data = factory.encounter_add_type(encounters_data, "Land", 21);
        version_index = encounters_file.object().value("dict_list").toArray().count(); //index is at the end of the file
    }

    int encounter_types = encounters_data.at(1).toObject().value("@types").toObject().value("dict_list").toArray().count();
    for (int i = 0; i < encounter_types; i++)
    {
        EncounterWidget *w = new EncounterWidget(encounters_data, i);
        this->ui->encounter_layout->addWidget(w);
        connect(w, &EncounterWidget::data_changed, [=]() {
            this->change_current(version_index,
                                 encounters_data.at(1).toObject().value("@map").toInt(),
                                 encounters_data.at(1).toObject().value("@version").toInt());
        });
        connect(w, &EncounterWidget::encounter_widget_deleted, [=]() {
            delete w;
            this->change_current(version_index,
                                 encounters_data.at(1).toObject().value("@map").toInt(),
                                 encounters_data.at(1).toObject().value("@version").toInt());
        });

    }
    if (create_new)
        this->change_current(version_index,this->current_id,version);
}


void MapEncounterDialog::on_button_cancel_clicked()
{
    this->close();
}


void MapEncounterDialog::on_button_ok_clicked()
{
    Writer writer(&encounters_file, db->data_dir + "encounters.dat", false, true);
}


void MapEncounterDialog::on_button_add_version_clicked()
{
    bool ok;
    int version = QInputDialog::getInt(this, "Version: ", "Version: ", 0, 0, 100,1, &ok);
    if (!ok)
        return;
    this->ui->combo_version->addItem(QString("Version %1").arg(version), -version-1); //due to 0
    this->ui->combo_version->setCurrentIndex(this->ui->combo_version->count()-1);

}


void MapEncounterDialog::on_button_remove_version_clicked()
{
    if (QMessageBox::question(this, "Delete Encounter version", "Do you really want to delete the current version?") != QMessageBox::Yes)
        return;
    bool ok;
    int version_index = this->ui->combo_version->currentData().toInt(&ok);
    if (!ok) return;

    QJsonObject obj = encounters_file.object();
    QJsonArray dict_list = obj.value("dict_list").toArray();
    dict_list.removeAt(version_index);
    obj.insert("dict_list", dict_list);
    encounters_file.setObject(obj);

    //trigger event manually to reload combo
    this->on_treeWidget_currentItemChanged(this->ui->treeWidget->currentItem(), 0);
}


void MapEncounterDialog::on_button_add_encounter_clicked()
{
    bool ok;
    int version_index = this->ui->combo_version->currentData().toInt(&ok);
    if (!ok) return;

    QJsonObject obj = encounters_file.object();
    QJsonArray dict_list = obj.value("dict_list").toArray();
    QJsonArray encounters_data = dict_list.at(version_index).toArray();
    encounters_data = PBSFactory().encounter_add_type(encounters_data, "Land", 21);
    dict_list.removeAt(version_index);
    dict_list.insert(version_index,encounters_data);
    obj.insert("dict_list", dict_list);
    encounters_file.setObject(obj);

    //trigger reload
    this->combo_version_changed();
}

