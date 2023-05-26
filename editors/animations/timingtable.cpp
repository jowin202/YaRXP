#include "timingtable.h"
#include "flashsedialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "RXIO2/factory.h"



TimingTable::TimingTable(QWidget *parent) : QTableWidget(parent)
{
    action_add = new QAction("Add");
    action_add->setShortcut(Qt::Key_Return);
    action_add->setShortcutContext(Qt::WidgetShortcut);
    connect(action_add, SIGNAL(triggered()), this, SLOT(add_timing()));
    this->addAction(action_add);

    action_edit = new QAction("Edit");
    action_edit->setShortcut(Qt::Key_Space);
    action_edit->setShortcutContext(Qt::WidgetShortcut);
    connect(action_edit, SIGNAL(triggered()), this, SLOT(edit_timing()));
    this->addAction(action_edit);

    action_delete = new QAction("Delete");
    action_delete->setShortcut(Qt::Key_Delete);
    action_delete->setShortcutContext(Qt::WidgetShortcut);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(delete_timing()));
    this->addAction(action_delete);

    action_copy = new QAction("Copy");
    action_copy->setShortcut(QKeySequence(tr("Ctrl+C")));
    action_copy->setShortcutContext(Qt::WidgetShortcut);
    connect(action_copy, SIGNAL(triggered()), this, SLOT(copy_timing()));
    this->addAction(action_copy);


    action_paste = new QAction("Paste");
    action_paste->setShortcut(QKeySequence(tr("Ctrl+V")));
    action_paste->setShortcutContext(Qt::WidgetShortcut);
    connect(action_paste, SIGNAL(triggered()), this, SLOT(paste_timing()));
    this->addAction(action_paste);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_customContextMenuRequested(const QPoint&)));
}


void TimingTable::update_timings()
{
    QJsonArray timings = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray();
    this->clearContents();
    this->setRowCount(timings.count());

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


        this->setItem(i,0, new QTableWidgetItem(QString("#%1").arg(timing.value("@frame").toInt()+1,3,10,QChar('0'))));
        this->setItem(i,1, new QTableWidgetItem(timing.value("@se").toObject().value("@name").toString()));
        this->setItem(i,2, new QTableWidgetItem(flash_text));
        this->setItem(i,3, new QTableWidgetItem(cond_text));

        this->set_readonly(this->item(i,0));
        this->set_readonly(this->item(i,1));
        this->set_readonly(this->item(i,2));
        this->set_readonly(this->item(i,3));
    }

    this->resizeColumnsToContents();
}

void TimingTable::add_timing()
{
    FlashSEDialog *dialog = new FlashSEDialog(this->ec,this->rowCount(), Factory().create_animation_timing());
    dialog->show();
    connect(dialog, &FlashSEDialog::ok_clicked, [=](QJsonObject obj, int row){
        QJsonArray timings = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray();
        //timings.removeAt(row);
        timings.insert(row,obj);
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@timings", timings);
        this->update_timings();
    });
}

void TimingTable::edit_timing()
{
    FlashSEDialog *dialog = new FlashSEDialog(this->ec,this->currentRow(),this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray().at(this->currentRow()).toObject());
    dialog->show();
    connect(dialog, &FlashSEDialog::ok_clicked, [=](QJsonObject obj, int row){
        QJsonArray timings = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray();
        timings.removeAt(row);
        timings.insert(row,obj);
        this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@timings", timings);
        this->update_timings();
    });
}

void TimingTable::copy_timing()
{
    int row = this->currentRow();
    QJsonObject timing = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray().at(row).toObject();
    QJsonDocument doc(timing);

    QSettings settings;
    settings.setValue("copied_animation_timing", doc.toJson(QJsonDocument::Compact));

}

void TimingTable::paste_timing()
{
    int row = this->currentRow();
    if (row < 0) row = 0;

    QSettings settings;
    QByteArray json = settings.value("copied_animation_timing").toByteArray();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
    if (err.error != QJsonParseError::NoError) return;

    QJsonArray timings = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray();
    timings.insert(row,doc.object());
    this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@timings", timings);

    this->update_timings();
    this->selectRow(row); //TODO Check this
}

void TimingTable::delete_timing()
{
    QJsonArray array = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@timings").toArray();
    array.removeAt(this->currentRow());
    this->ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@timings", array);
    this->update_timings();
}


void TimingTable::on_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *item = this->itemAt(pos);
    if (item) {
        QMenu menu;
        menu.addAction(action_add);
        menu.addAction(action_edit);
        menu.addAction(action_delete);
        menu.addSeparator();
        menu.addAction(action_copy);
        menu.addAction(action_paste);

        menu.exec(this->mapToGlobal(pos));
    }
}

