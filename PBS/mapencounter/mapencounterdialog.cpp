#include "mapencounterdialog.h"
#include "ui_mapencounterdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/rpgmapcontroller.h"

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
    Q_UNUSED(previous);
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
}

void MapEncounterDialog::combo_version_changed()
{
    bool ok;
    int version_index = this->ui->combo_version->currentData().toInt(&ok);
    if (!ok) return;

    QLayoutItem *item;
     while ( (item = this->ui->encounter_layout->takeAt(0)) != 0)
         delete item->widget();

    QJsonArray encounters_data = encounters_file.object().value("dict_list").toArray().at(version_index).toArray();
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

    }
}

