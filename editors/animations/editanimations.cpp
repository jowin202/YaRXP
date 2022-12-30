#include "editanimations.h"
#include "ui_editanimations.h"

#include "flashsedialog.h"

#include "dialogs/imagedialog.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

EditAnimations::EditAnimations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditAnimations)
{
    ui->setupUi(this);
}

EditAnimations::~EditAnimations()
{
    delete ui;
}

void EditAnimations::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ui->animation_label->setEC(ec);
    this->ui->table_timings->setEC(ec);
    this->ui->graphic_preview_label->setDB(ec->get_db());

    this->ec->connect_string_to_text_field(RPGDB::ANIMATIONS, "@name", this->ui->line_name);
    this->ec->connect_string_to_text_field(RPGDB::ANIMATIONS, "@animation_name", this->ui->line_animation);
    this->ec->connect_int_to_combo_box(RPGDB::ANIMATIONS, "@position", this->ui->combo_pos);

    connect(this->ui->combo_pos, &QComboBox::currentIndexChanged, this, [=]() { this->ui->animation_label->update(this->ui->frame_list->currentRow()); });

    connect(this->ec, &RPGEditorController::current_animation_changed, this, [=]() {
        this->ui->line_max_frames->setText(QString::number(this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().count()));
        this->update_frame_list();
        this->ui->frame_list->setCurrentRow(0);
        //this->ui->animation_label->update(0);
        this->ui->table_timings->update_timings();
        this->ui->graphic_preview_label->set_image(this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@animation_name").toString(), this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@animation_hue").toInt());
        //qDebug() << this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings");
    });


}

void EditAnimations::on_button_animation_clicked()
{
    ImageDialog *imdialog = new ImageDialog(ec->get_db(), ImageDialog::ANIMATIONS,
                                            this->ui->line_animation->text());
    imdialog->set_hue(this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@animation_hue").toInt());
    connect(imdialog, SIGNAL(ok_clicked(QString)), this->ui->line_animation, SLOT(setText(QString)));
    connect(imdialog, &ImageDialog::ok_clicked_with_hue, this, [=](int hue){
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@animation_hue",hue);
        this->ec->refresh(RPGDB::ANIMATIONS); //update view
    } );
    imdialog->show();
}


void EditAnimations::on_button_set_frames_clicked()
{
    bool ok;
    int frames = QInputDialog::getInt(this, "Frame Count", "Set Frame Count:", this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().count(), 1, 200, 1, &ok);
    if (ok)
    {
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frame_max", frames);
        //TODO: insert frames
        this->ec->refresh(RPGDB::ANIMATIONS); //update view
    }
}

void EditAnimations::update_frame_list()
{
    int num_frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().count();
    this->ui->frame_list->clear();
    for (int i = 0; i < num_frames; i++)
    {
        this->ui->frame_list->addItem(QString("#%1").arg(i+1,3,10,QChar('0')));
    }
}

void EditAnimations::on_table_timings_itemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    this->ui->table_timings->edit_timing();
}




void EditAnimations::on_frame_list_currentRowChanged(int currentRow)
{
    QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
    QJsonObject frame = frames.at(currentRow).toObject();

    this->ui->animation_label->update(currentRow);
    qDebug() << frame;
}


void EditAnimations::on_button_edit_battler_clicked()
{
    QString battler_name = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battler_name").toString();
    int battler_hue = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battler_hue").toInt();

    ImageDialog *imdialog = new ImageDialog(ec->get_db(),ImageDialog::BATTLERS, battler_name);
    imdialog->set_hue(battler_hue);
    connect(imdialog, &ImageDialog::ok_clicked, this, [=](QString battler_name){ this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@battler_name", battler_name); });
    connect(imdialog, &ImageDialog::ok_clicked_with_hue, this, [=](int battler_hue ){ this->ec->obj_set_jsonvalue(RPGDB::SYSTEM, "@battler_hue", battler_hue); });
    connect(imdialog, &ImageDialog::ok_clicked, this, [=](){ this->ui->animation_label->update(this->ui->frame_list->currentRow()); });
    imdialog->show();
}



void EditAnimations::on_button_paste_last_clicked()
{
    int current_frame = this->ui->frame_list->currentRow();
    if (current_frame <= 0) return; //cant paste previous frame

    QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
    frames.removeAt(current_frame);
    frames.insert(current_frame, frames.at(current_frame-1));
    this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frames);
    this->ui->animation_label->update(current_frame);
}


void EditAnimations::on_button_clear_frame_clicked()
{
    //TODO
    /*
    int current_frame = this->ui->frame_list->currentRow();
    if (current_frame < 0) return; //

    QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
    frames.removeAt(current_frame);
    frames.insert(current_frame, frames.at(current_frame-1));
    this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frames);
    this->ui->animation_label->update(current_frame);
    */
}

