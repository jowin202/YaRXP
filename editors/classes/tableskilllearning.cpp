#include "tableskilllearning.h"
#include "skilllearning.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "RXIO2/factory.h"


TableSkillLearning::TableSkillLearning(QWidget *parent) : QTableWidget(parent)
{
    action_add = new QAction("Add");
    action_add->setShortcut(Qt::Key_Return);
    action_add->setShortcutContext(Qt::WidgetShortcut);
    connect(action_add, SIGNAL(triggered()), this, SLOT(item_add()));
    this->addAction(action_add);

    action_edit = new QAction("Edit");
    action_edit->setShortcut(Qt::Key_Space);
    action_edit->setShortcutContext(Qt::WidgetShortcut);
    connect(action_edit, SIGNAL(triggered()), this, SLOT(item_edit()));
    this->addAction(action_edit);

    action_delete = new QAction("Delete");
    action_delete->setShortcut(Qt::Key_Delete);
    action_delete->setShortcutContext(Qt::WidgetShortcut);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(item_delete()));
    this->addAction(action_delete);

    action_copy = new QAction("Copy");
    action_copy->setShortcut(QKeySequence(tr("Ctrl+C")));
    action_copy->setShortcutContext(Qt::WidgetShortcut);
    connect(action_copy, SIGNAL(triggered()), this, SLOT(item_copy()));
    this->addAction(action_copy);


    action_paste = new QAction("Paste");
    action_paste->setShortcut(QKeySequence(tr("Ctrl+V")));
    action_paste->setShortcutContext(Qt::WidgetShortcut);
    connect(action_paste, SIGNAL(triggered()), this, SLOT(item_paste()));
    this->addAction(action_paste);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_customContextMenuRequested(const QPoint&)));
}

void TableSkillLearning::on_customContextMenuRequested(const QPoint &pos)
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

void TableSkillLearning::update_skill_from_file()
{
    this->clearContents();
    QJsonArray learnings = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray();
    QStringList skill_names = this->ec->obj_get_name_list(RPGDB::SKILLS);
    int rowCount = learnings.count();
    this->setRowCount(rowCount);


    for (int row = 0; row < rowCount; row++)
    {
        int level = learnings.at(row).toObject().value("@level").toInt();
        int skill_id = learnings.at(row).toObject().value("@skill_id").toInt();


        QString skill_name;
        if (skill_id > 0)
            skill_name = skill_id <= skill_names.count() ? skill_names.at(skill_id-1) : "";

        this->setItem(row,0,new QTableWidgetItem("Lv. " + QString::number(level)));
        this->setItem(row,1,new QTableWidgetItem(QString("%1: %2").arg(skill_id,3,10,QChar('0')).arg(skill_name)));

        this->set_readonly(this->item(row,0));
        this->set_readonly(this->item(row,1));
    }

    this->resizeColumnsToContents();
}


void TableSkillLearning::set_skill_from_dialog(int row, int level, int skill)
{
    QJsonArray learnings = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray();
    learnings.removeAt(row);
    learnings.insert(row, Factory().create_learnings(level,skill));
    this->ec->obj_set_jsonvalue(RPGDB::CLASSES, "@learnings", learnings);

    this->update_skill_from_file();
    this->selectRow(row); //TODO Check this
}


void TableSkillLearning::item_add()
{
    SkillLearning *dialog = new SkillLearning(ec, this->rowCount(), 1, 1, 0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int)), this, SLOT(set_skill_from_dialog(int,int,int)));
    dialog->show();
}

void TableSkillLearning::item_delete()
{
    int row = this->currentRow();
    if (row < 0) return;

    QJsonArray learnings = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray();
    learnings.removeAt(row);
    this->ec->obj_set_jsonvalue(RPGDB::CLASSES, "@learnings", learnings);

    this->update_skill_from_file();
}

void TableSkillLearning::item_copy()
{
    int row = this->currentRow();
    if (row < 0) return;
    QJsonObject learning = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray().at(row).toObject();
    QJsonDocument doc(learning);

    QSettings settings;
    settings.setValue("copied_item_skill_learning", doc.toJson(QJsonDocument::Compact));
}

void TableSkillLearning::item_paste()
{
    int row = this->currentRow();
    if (row < 0) row = 0;

    QSettings settings;
    QByteArray json = settings.value("copied_item_skill_learning").toByteArray();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
    if (err.error != QJsonParseError::NoError) return;

    if (doc.object().value("@skill_id").toInt() <= 0) return; //invalid skill id


    QJsonArray learnings = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray();
    learnings.insert(row,doc.object());
    this->ec->obj_set_jsonvalue(RPGDB::CLASSES, "@learnings", learnings);

    this->update_skill_from_file();
    this->selectRow(row); //TODO Check this
}

void TableSkillLearning::item_edit()
{
    int row = this->currentRow();
    if (row < 0) return;

    QJsonObject learning = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray().at(row).toObject();

    int level = learning.value("@level").toInt();
    int skill = learning.value("@skill_id").toInt();

    SkillLearning *dialog = new SkillLearning(ec,row,level,skill, 0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int)), this, SLOT(set_skill_from_dialog(int,int,int)));
    dialog->show();
}
