#include "encounterwidget.h"
#include "ui_encounterwidget.h"

#include "encounterslot.h"

EncounterWidget::EncounterWidget(QJsonArray encounters_data, int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EncounterWidget)
{
    ui->setupUi(this);
    this->encounters_data = encounters_data;
    this->index = index;

    QJsonArray current = this->encounters_data.at(1).toObject().value("@step_chances").toObject().value("dict_list").toArray().at(index).toArray();
    this->ui->combo_type->setCurrentText(current.at(0).toObject().value("symbol_value").toString());
    this->ui->spin_step_chance->setValue(current.at(1).toInt());

    this->update_slots();
}

EncounterWidget::~EncounterWidget()
{
    delete ui;
}

void EncounterWidget::update_slots()
{
    QLayoutItem *item;
     while ( (item = this->ui->encounter_layout->takeAt(0)) != 0)
         delete item->widget();

    QJsonArray types = encounters_data.at(1).toObject().value("@types").toObject().value("dict_list").toArray().at(index).toArray().at(1).toArray();
    for (int i = 0; i < types.count(); i++)
    {
        EncounterSlot *s = new EncounterSlot(types.at(i).toArray());
        this->ui->encounter_layout->addWidget(s);

        connect(s, &EncounterSlot::content_changed, [=](QJsonArray array){
            QJsonObject obj = this->encounters_data.at(1).toObject();
            QJsonObject types_obj = obj.value("@types").toObject();
            QJsonArray types_dict_list = types_obj.value("dict_list").toArray();
            QJsonArray types_dict_list_at_index = types_dict_list.at(index).toArray();
            QJsonArray types_dict_list_at_index_at_1 = types_dict_list_at_index.at(1).toArray();
            types_dict_list_at_index_at_1.removeAt(i);
            types_dict_list_at_index_at_1.insert(i,array);

            types_dict_list_at_index.removeAt(1);
            types_dict_list_at_index.insert(1,types_dict_list_at_index_at_1);

            types_dict_list.removeAt(index);
            types_dict_list.insert(index, types_dict_list_at_index);

            types_obj.insert("dict_list", types_dict_list);
            obj.insert("@types", types_obj);
            this->encounters_data.removeAt(1);
            this->encounters_data.insert(1,obj);
            emit data_changed(this->encounters_data);
        });


        connect(s, &EncounterSlot::slot_deleted, [=](){
            QJsonObject obj = this->encounters_data.at(1).toObject();
            QJsonObject types_obj = obj.value("@types").toObject();
            QJsonArray types_dict_list = types_obj.value("dict_list").toArray();
            QJsonArray types_dict_list_at_index = types_dict_list.at(index).toArray();
            QJsonArray types_dict_list_at_index_at_1 = types_dict_list_at_index.at(1).toArray();
            types_dict_list_at_index_at_1.removeAt(i);

            types_dict_list_at_index.removeAt(1);
            types_dict_list_at_index.insert(1,types_dict_list_at_index_at_1);

            types_dict_list.removeAt(index);
            types_dict_list.insert(index, types_dict_list_at_index);

            types_obj.insert("dict_list", types_dict_list);
            obj.insert("@types", types_obj);
            this->encounters_data.removeAt(1);
            this->encounters_data.insert(1,obj);
            emit data_changed(this->encounters_data);
            this->update_slots();
        });
    }
}
