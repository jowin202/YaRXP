#include "editanimations.h"
#include "ui_editanimations.h"

#include "flashsedialog.h"
#include "copyframesdialog.h"
#include "tweeningdialog.h"
#include "cellbatchdialog.h"
#include "entireslidedialog.h"

#include "dialogs/imagedialog.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

EditAnimations::EditAnimations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditAnimations)
{
    ui->setupUi(this);

    connect(this->ui->graphic_preview_label, &AnimationGraphicPreview::max_pattern, [=](int m) { this->ui->animation_label->set_max_pattern(m); });
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


    //click on the preview to set pattern for the label
    connect(this->ui->graphic_preview_label, SIGNAL(cell_chosen(int)), this->ui->animation_label, SLOT(set_pattern(int)));
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
    int frames_count = QInputDialog::getInt(this, "Frame Count", "Set Frame Count:", this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().count(), 1, 200, 1, &ok);
    if (ok)
    {
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frame_max", frames_count);
        QJsonArray frames = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();

        while (frames.count() > frames_count)
        {
            frames.removeLast();
        }
        while (frames.count() < frames_count)
        {
            frames.append(Factory().create_animation_frame());
        }


        ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frames);
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
    //qDebug() << frame;
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



void EditAnimations::on_button_copy_clicked()
{
    CopyFramesDialog *dialog = new CopyFramesDialog(this->ui->frame_list->count(),true);
    dialog->show();
    connect(dialog, &CopyFramesDialog::ok_clicked, [=](int from, int to, int dest){
        int min_frame = qMin(from,to);
        int max_frame = qMax(from, to);
        int delta = dest-from;

        QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
        for (int i = min_frame; i <= max_frame && (i+delta) >= 1 && (i+delta) <= frames.count(); i++)
        {
            QJsonObject from_frame = frames.at(i-1).toObject(); //starts counting at 1
            frames.removeAt(i-1+delta);
            frames.insert(i-1+delta,from_frame);
        }
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frames);
        this->ui->animation_label->update(this->ui->frame_list->currentRow());
    });
}


void EditAnimations::on_button_clear_frames_clicked()
{
    CopyFramesDialog *dialog = new CopyFramesDialog(this->ui->frame_list->count(),false);
    dialog->show();
    connect(dialog, &CopyFramesDialog::ok_clicked, [=](int from, int to, int dest){
        Q_UNUSED(dest);
        int min_frame = qMin(from,to);
        int max_frame = qMax(from, to);

        QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
        for (int i = min_frame; i <= max_frame; i++)
        {
            QJsonObject frame = frames.at(i-1).toObject(); //starts counting at 1
            QJsonObject cell_data = frame.value("@cell_data").toObject();
            cell_data.insert("values", QJsonArray());
            cell_data.insert("x", 0);
            frame.insert("@cell_data", cell_data);
            frame.insert("@cell_max", 0);
            frames.removeAt(i-1);
            frames.insert(i-1,frame);
        }
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frames);
        this->ui->animation_label->update(this->ui->frame_list->currentRow());
    });
}


void EditAnimations::on_button_tweening_clicked()
{
    TweeningDialog *dialog = new TweeningDialog(this->ui->frame_list->count());
    dialog->show();
    connect(dialog, &TweeningDialog::ok_clicked, [=](int from_frame, int to_frame, int from_cell, int to_cell,
                                                      bool pattern, bool position, bool opacity){
        int min_frame = qMin(from_frame,to_frame);
        int max_frame = qMax(from_frame, to_frame);
        int min_cell = qMin(from_cell,to_cell);
        int max_cell = qMax(from_cell, to_cell);
        int delta = max_frame - min_frame;
        if (delta == 0) return;

        QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
        QJsonArray first_frame_cell_values = frames.at(min_frame-1).toObject().value("@cell_data").toObject().value("values").toArray();
        QJsonArray last_frame_cell_values = frames.at(max_frame-1).toObject().value("@cell_data").toObject().value("values").toArray();
        int first_frame_cell_max = frames.at(min_frame-1).toObject().value("@cell_max").toInt();
        int last_frame_cell_max = frames.at(max_frame-1).toObject().value("@cell_max").toInt();

        max_cell = qMin(max_cell, first_frame_cell_max); // only do cells which are in the first frame

        int cnt = 0;
        for (int i = min_frame+1; i < max_frame && i < frames.count(); i++) //dont change first and last
        {
            cnt++;
            QJsonObject current_frame = frames.at(i-1).toObject(); //starts counting at 1
            QJsonObject current_cell_data = current_frame.value("@cell_data").toObject();
            QJsonArray current_cell_data_values = current_cell_data.value("values").toArray();
            int current_frame_cell_max = current_frame.value("@cell_max").toInt();

            while (current_frame_cell_max < max_cell)
            {
                current_cell_data_values.insert(8*current_frame_cell_max, 1);   //blending
                current_cell_data_values.insert(7*current_frame_cell_max, 255);   //opacity
                current_cell_data_values.insert(6*current_frame_cell_max, 0);   //flip
                current_cell_data_values.insert(5*current_frame_cell_max, 0);   //angle
                current_cell_data_values.insert(4*current_frame_cell_max, 100); //zoom
                current_cell_data_values.insert(3*current_frame_cell_max,0);
                current_cell_data_values.insert(2*current_frame_cell_max,0);
                current_cell_data_values.insert(1*current_frame_cell_max,1);

                //increase cell_max
                current_frame.insert("@cell_max", current_frame_cell_max+1);
                current_cell_data.insert("x", current_frame_cell_max+1);
                current_frame_cell_max = current_frame.value("@cell_max").toInt();
            }

            for (int j = min_cell-1; j < max_cell; j++) //counting at 0, but in dialog counting at 1
            {
                if (position) //position, zoom, angle
                {
                    int x = first_frame_cell_values.at(1*first_frame_cell_max+j).toInt();
                    int y = first_frame_cell_values.at(2*last_frame_cell_max+j).toInt();
                    int zoom = first_frame_cell_values.at(3*last_frame_cell_max+j).toInt();
                    int angle = first_frame_cell_values.at(4*last_frame_cell_max+j).toInt();
                    qreal delta_x = (last_frame_cell_values.at(1*last_frame_cell_max+j).toInt() - first_frame_cell_values.at(1*first_frame_cell_max+j).toInt())/(1.0*delta);
                    qreal delta_y = (last_frame_cell_values.at(2*last_frame_cell_max+j).toInt() - first_frame_cell_values.at(2*first_frame_cell_max+j).toInt())/(1.0*delta);
                    qreal delta_zoom = (last_frame_cell_values.at(3*last_frame_cell_max+j).toInt() - first_frame_cell_values.at(3*first_frame_cell_max+j).toInt())/(1.0*delta);
                    qreal delta_angle = (last_frame_cell_values.at(4*last_frame_cell_max+j).toInt() - first_frame_cell_values.at(4*first_frame_cell_max+j).toInt())/(1.0*delta);
                    current_cell_data_values.removeAt(1*current_frame_cell_max+j);
                    current_cell_data_values.insert(1*current_frame_cell_max+j,custom_round(x+cnt*delta_x));
                    current_cell_data_values.removeAt(2*current_frame_cell_max+j);
                    current_cell_data_values.insert(2*current_frame_cell_max+j,custom_round(y+cnt*delta_y));
                    current_cell_data_values.removeAt(3*current_frame_cell_max+j);
                    current_cell_data_values.insert(3*current_frame_cell_max+j,custom_round(zoom+cnt*delta_zoom));
                    current_cell_data_values.removeAt(4*current_frame_cell_max+j);
                    current_cell_data_values.insert(4*current_frame_cell_max+j,custom_round(angle+cnt*delta_angle));
                    //qDebug() << i << custom_round(x+cnt*delta_x) << custom_round(y+cnt*delta_y) << custom_round(zoom+cnt*delta_zoom);
                }
                if (pattern)
                {
                    int pattern = first_frame_cell_values.at(0*first_frame_cell_max+j).toInt();
                    qreal delta_pattern = (last_frame_cell_values.at(0*last_frame_cell_max+j).toInt() - first_frame_cell_values.at(0*first_frame_cell_max+j).toInt())/(1.0*delta);

                    current_cell_data_values.removeAt(0*current_frame_cell_max+j);
                    current_cell_data_values.insert(0*current_frame_cell_max+j,custom_round(pattern+cnt*delta_pattern));
                }
                if (opacity) //opacity, blending
                {
                    int opacity = first_frame_cell_values.at(6*first_frame_cell_max+j).toInt();
                    int blending = first_frame_cell_values.at(7*first_frame_cell_max+j).toInt();
                    qreal delta_opacity = (last_frame_cell_values.at(6*last_frame_cell_max+j).toInt() - first_frame_cell_values.at(6*first_frame_cell_max+j).toInt())/(1.0*delta);
                    qreal delta_blending = (last_frame_cell_values.at(7*last_frame_cell_max+j).toInt() - first_frame_cell_values.at(7*first_frame_cell_max+j).toInt())/(1.0*delta);

                    current_cell_data_values.removeAt(6*current_frame_cell_max+j);
                    current_cell_data_values.insert(6*current_frame_cell_max+j,custom_round(opacity+cnt*delta_opacity));
                    current_cell_data_values.removeAt(7*current_frame_cell_max+j);
                    current_cell_data_values.insert(7*current_frame_cell_max+j,custom_round(blending+cnt*delta_blending));
                }
            }
            current_cell_data.insert("values", current_cell_data_values);
            current_frame.insert("@cell_data", current_cell_data);
            frames.removeAt(i-1);
            frames.insert(i-1,current_frame);
        }
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frames);
        this->ui->animation_label->update(this->ui->frame_list->currentRow());
    });
}


void EditAnimations::on_button_cell_batch_clicked()
{
    CellBatchDialog *dialog = new CellBatchDialog(this->ui->frame_list->count());
    dialog->show();
    connect(dialog, &CellBatchDialog::ok_clicked, [=](int from_frame, int to_frame, int from_cell, int to_cell,
                                                      int pattern, int x, int y, int zoom,
                                                      int angle, int flip, int opacity, int blending){
        int min_frame = qMin(from_frame,to_frame);
        int max_frame = qMax(from_frame, to_frame);
        int min_cell = qMin(from_cell,to_cell);
        int max_cell = qMax(from_cell, to_cell);


        QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
        for (int i = min_frame; i <= max_frame && i <= frames.count(); i++)
        {
            QJsonObject current_frame = frames.at(i-1).toObject(); //starts counting at 1
            QJsonObject current_cell_data = current_frame.value("@cell_data").toObject();
            QJsonArray current_cell_data_values = current_cell_data.value("values").toArray();
            int current_frame_cell_max = current_frame.value("@cell_max").toInt();

            for (int j = min_cell-1; j < max_cell && j < current_frame_cell_max; j++) //counting at 0, but in dialog counting at 1
            {
                if (pattern >=0)
                {
                    current_cell_data_values.removeAt(0*current_frame_cell_max+j);
                    current_cell_data_values.insert(0*current_frame_cell_max+j,pattern-1); //start counting at 1 (wtf)
                }
                if (x >=0)
                {
                    current_cell_data_values.removeAt(1*current_frame_cell_max+j);
                    current_cell_data_values.insert(1*current_frame_cell_max+j,x);
                }
                if (y >=0)
                {
                    current_cell_data_values.removeAt(2*current_frame_cell_max+j);
                    current_cell_data_values.insert(2*current_frame_cell_max+j,y);
                }
                if (zoom >=0)
                {
                    current_cell_data_values.removeAt(3*current_frame_cell_max+j);
                    current_cell_data_values.insert(3*current_frame_cell_max+j,zoom);
                }
                if (angle >=0)
                {
                    current_cell_data_values.removeAt(4*current_frame_cell_max+j);
                    current_cell_data_values.insert(4*current_frame_cell_max+j,angle);
                }
                if (flip >=0)
                {
                    current_cell_data_values.removeAt(5*current_frame_cell_max+j);
                    current_cell_data_values.insert(5*current_frame_cell_max+j,flip);
                }
                if (opacity >=0)
                {
                    current_cell_data_values.removeAt(6*current_frame_cell_max+j);
                    current_cell_data_values.insert(6*current_frame_cell_max+j,opacity);
                }
                if (blending >=0)
                {
                    current_cell_data_values.removeAt(7*current_frame_cell_max+j);
                    current_cell_data_values.insert(7*current_frame_cell_max+j,blending);
                }
            }
            current_cell_data.insert("values", current_cell_data_values);
            current_frame.insert("@cell_data", current_cell_data);
            frames.removeAt(i-1);
            frames.insert(i-1,current_frame);
        }
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frames);
        this->ui->animation_label->update(this->ui->frame_list->currentRow());
    });
}


void EditAnimations::on_button_entire_slide_clicked()
{
    EntireSlideDialog *dialog = new EntireSlideDialog(this->ui->frame_list->count());
    dialog->show();
    connect(dialog, &EntireSlideDialog::ok_clicked, [=](int from_frame, int to_frame,
                                                        int x, int y){
        int min_frame = qMin(from_frame,to_frame);
        int max_frame = qMax(from_frame, to_frame);

        QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
        for (int i = min_frame; i <= max_frame && i <= frames.count(); i++)
        {
            QJsonObject current_frame = frames.at(i-1).toObject(); //starts counting at 1
            QJsonObject current_cell_data = current_frame.value("@cell_data").toObject();
            QJsonArray current_cell_data_values = current_cell_data.value("values").toArray();
            int current_frame_cell_max = current_frame.value("@cell_max").toInt();

            for (int j = 0; j < current_frame_cell_max; j++)
            {
                if (x != 0)
                {
                    int tmp = current_cell_data_values.at(1*current_frame_cell_max+j).toInt();
                    current_cell_data_values.removeAt(1*current_frame_cell_max+j);
                    current_cell_data_values.insert(1*current_frame_cell_max+j,tmp+x);
                }
                if (y != 0)
                {
                    int tmp = current_cell_data_values.at(2*current_frame_cell_max+j).toInt();
                    current_cell_data_values.removeAt(2*current_frame_cell_max+j);
                    current_cell_data_values.insert(2*current_frame_cell_max+j,tmp+y);
                }
            }
            current_cell_data.insert("values", current_cell_data_values);
            current_frame.insert("@cell_data", current_cell_data);
            frames.removeAt(i-1);
            frames.insert(i-1,current_frame);
        }
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frames);
        this->ui->animation_label->update(this->ui->frame_list->currentRow());
    });
}

