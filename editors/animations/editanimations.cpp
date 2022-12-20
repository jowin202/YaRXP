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

    action_add = new QAction("Add");
    action_add->setShortcut(Qt::Key_Return);
    connect(action_add, SIGNAL(triggered()), this, SLOT(add_timing()));
    this->ui->table_timings->addAction(action_add);

    action_edit = new QAction("Edit");
    action_edit->setShortcut(Qt::Key_Space);
    connect(action_edit, SIGNAL(triggered()), this, SLOT(edit_timing()));
    this->ui->table_timings->addAction(action_edit);

    action_delete = new QAction("Delete");
    action_delete->setShortcut(Qt::Key_Delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(delete_timing()));
    this->ui->table_timings->addAction(action_delete);

    action_copy = new QAction("Copy");
    action_copy->setShortcut(QKeySequence(tr("Ctrl+C")));
    connect(action_copy, SIGNAL(triggered()), this, SLOT(copy_timing()));
    this->ui->table_timings->addAction(action_copy);


    action_paste = new QAction("Paste");
    action_paste->setShortcut(QKeySequence(tr("Ctrl+V")));
    connect(action_paste, SIGNAL(triggered()), this, SLOT(paste_timing()));
    this->ui->table_timings->addAction(action_paste);

}

EditAnimations::~EditAnimations()
{
    delete ui;
}

void EditAnimations::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ui->graphic_preview_label->setDB(ec->get_db());

    this->ec->connect_string_to_text_field(RPGDB::ANIMATIONS, "@name", this->ui->line_name);
    this->ec->connect_string_to_text_field(RPGDB::ANIMATIONS, "@animation_name", this->ui->line_animation);
    this->ec->connect_int_to_combo_box(RPGDB::ANIMATIONS, "@position", this->ui->combo_pos);

    connect(this->ec, &RPGEditorController::current_animation_changed, this, [=]() {
        this->ui->line_max_frames->setText(QString::number(this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().count()));
        this->update_frame_list();
        this->update_timings();
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

void EditAnimations::update_timings()
{
    QJsonArray timings = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray();
    this->ui->table_timings->clearContents();
    this->ui->table_timings->setRowCount(timings.count());

    for (int i = 0; i < timings.count(); i++)
    {
        QJsonObject timing = timings.at(i).toObject();
        QJsonObject flash_color = timing.value("@flash_color").toObject();


        QString flash_text;
        switch (timing.value("@flash_scope").toInt())
        {
        case 1: flash_text = QString("Target (%1,%2,%3,%4)").arg(flash_color.value("r").toInt()).arg(flash_color.value("g").toInt()).arg(flash_color.value("b").toInt()).arg(flash_color.value("alpha_gray").toInt()); break;
        case 2: flash_text = QString("Screen (%1,%2,%3,%4)").arg(flash_color.value("r").toInt()).arg(flash_color.value("g").toInt()).arg(flash_color.value("b").toInt()).arg(flash_color.value("alpha_gray").toInt()); break;
        case 3: flash_text = "Hide Target"; break;
        }
        if (timing.value("@flash_scope").toInt() != 0)
            flash_text.append(", @" + QString::number(timing.value("@flash_duration").toInt()));

        QString cond_text;
        if (timing.value("@condition").toInt() == 0) cond_text = "None";
        else if (timing.value("@condition").toInt() == 1) cond_text = "Hit";
        else if (timing.value("@condition").toInt() == 2) cond_text = "Miss";


        this->ui->table_timings->setItem(i,0, new QTableWidgetItem(QString("#%1").arg(timing.value("@frame").toInt()+1,3,10,QChar('0'))));
        this->ui->table_timings->setItem(i,1, new QTableWidgetItem(timing.value("@se").toObject().value("@name").toString()));
        this->ui->table_timings->setItem(i,2, new QTableWidgetItem(flash_text));
        this->ui->table_timings->setItem(i,3, new QTableWidgetItem(cond_text));

        this->set_readonly(this->ui->table_timings->item(i,0));
        this->set_readonly(this->ui->table_timings->item(i,1));
        this->set_readonly(this->ui->table_timings->item(i,2));
        this->set_readonly(this->ui->table_timings->item(i,3));
    }

    this->ui->table_timings->resizeColumnsToContents();
}


void EditAnimations::on_table_timings_itemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    this->edit_timing();
}

void EditAnimations::add_timing()
{

}

void EditAnimations::edit_timing()
{
    FlashSEDialog *dialog = new FlashSEDialog(this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray().at(this->ui->table_timings->currentRow()).toObject());
    dialog->show();
}

void EditAnimations::copy_timing()
{

}

void EditAnimations::paste_timing()
{

}

void EditAnimations::delete_timing()
{
    QJsonArray array = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray();
    array.removeAt(this->ui->table_timings->currentRow());
    this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@timings", array);
    this->ec->refresh(RPGDB::ANIMATIONS); //update view
}


void EditAnimations::on_table_timings_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *item = this->ui->table_timings->itemAt(pos);
    if (item) {
        QMenu menu;
        menu.addAction(action_add);
        menu.addAction(action_edit);
        menu.addAction(action_delete);
        menu.addSeparator();
        menu.addAction(action_copy);
        menu.addAction(action_paste);

        menu.exec(this->ui->table_timings->mapToGlobal(pos));
    }
}

