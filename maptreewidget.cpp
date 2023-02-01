#include "maptreewidget.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"

#include "import/importdialog.h"

MapTreeWidget::MapTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(clicked_at_item(QTreeWidgetItem*)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepare_context_menu(QPoint)));

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(show_map_properties_dialog()));

    action1.setText("&Map properties");
    action1.setShortcut(QKeySequence(Qt::Key_Space));
    action1.setShortcutContext(Qt::WidgetShortcut);
    connect(&action1, SIGNAL(triggered()), this, SLOT(show_map_properties_dialog()));
    action2.setText("&New Map");
    action2.setShortcut(QKeySequence(Qt::Key_Insert));
    action2.setShortcutContext(Qt::WidgetShortcut);
    this->addAction(&action2);
    connect(&action2, SIGNAL(triggered()), this, SLOT(create_new_map()));
    action3.setText("&Copy");
    action3.setShortcut(QKeySequence(tr("Ctrl+C")));
    action3.setShortcutContext(Qt::WidgetShortcut);
    connect(&action3, SIGNAL(triggered()), this, SLOT(copy_map()));
    action4.setText("&Paste");
    action4.setShortcut(QKeySequence(tr("Ctrl+V")));
    action4.setShortcutContext(Qt::WidgetShortcut);
    connect(&action4, SIGNAL(triggered()), this, SLOT(paste_map()));
    action5.setText("&Delete");
    action5.setShortcut(QKeySequence(Qt::Key_Delete));
    action5.setShortcutContext(Qt::WidgetShortcut);
    connect(&action5, SIGNAL(triggered()), this, SLOT(delete_map()));
    action6.setText("&Import");
    action6.setShortcutContext(Qt::WidgetShortcut);
    connect(&action6, &QAction::triggered, [=](){ ImportDialog *dialog = new ImportDialog(db); dialog->show();});



    menu.addAction(&action1);
    this->addAction(&action1); //for shortcut
    menu.addSeparator();
    menu.addAction(&action2);
    menu.addSeparator();
    menu.addAction(&action3);
    menu.addAction(&action4);
    menu.addAction(&action5);
    menu.addSeparator();
    menu.addAction(&action6);


    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(itemExpanded(QTreeWidgetItem*)));
    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(itemCollapsed(QTreeWidgetItem*)));
}

void MapTreeWidget::setDB(RPGDB *db)
{
    this->db = db;
    this->mic = new RPGMapInfoController(db);
}

void MapTreeWidget::restore_after_move()
{
    int order = 1;
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        this->restore_order_and_parent(this->topLevelItem(i), &order);
        int id = this->topLevelItem(i)->text(2).toInt();
        this->mic->set_parent(id, 0); //toplevel item has parent 0
    }
    this->list_maps();
}

void MapTreeWidget::restore_order_and_parent(QTreeWidgetItem *item, int *current)
{
    int id = item->text(2).toInt();
    //item->setText(1,QString("%1").arg((*current),3,10,QChar('0')));
    mic->set_order(id, (*current)++);

    for (int i = 0; i < item->childCount(); i++)
    {
        int child_id = item->child(i)->text(2).toInt();
        this->mic->set_parent(child_id, id);
        this->restore_order_and_parent(item->child(i), current);
    }
}

void MapTreeWidget::list_maps()
{
    int scroll = this->verticalScrollBar()->value();
    int last_id = -1;
    if (this->currentItem() != 0)
        last_id = this->currentItem()->text(2).toInt();

    id_map.clear();
    QList<QTreeWidgetItem*> items_to_delete = this->findItems("", Qt::MatchContains, 0); //delete them later


    QJsonArray toplevel_maps = this->mic->get_toplevel_maps();
    QJsonArray non_toplevel_maps = this->mic->get_child_maps();

    for (int i = 0; i < toplevel_maps.count(); i++)
    {
        QStringList columns;
        columns << toplevel_maps.at(i).toObject().value("@name").toString();
        columns << QString::number(toplevel_maps.at(i).toObject().value("@order").toInt()).rightJustified(3,'0');
        columns << QString::number(toplevel_maps.at(i).toObject().value("id").toInt());
        QTreeWidgetItem *item = new QTreeWidgetItem(columns);
        this->addTopLevelItem(item);
        this->id_map.insert(toplevel_maps.at(i).toObject().value("id").toInt(), item);
        if (last_id == toplevel_maps.at(i).toObject().value("id").toInt())
            this->setCurrentItem(item);
        if (toplevel_maps.at(i).toObject().value("@expanded").toBool())
            this->expandItem(item);
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

                if (last_id == non_toplevel_maps.at(i).toObject().value("id").toInt())
                    this->setCurrentItem(item);
                if (non_toplevel_maps.at(i).toObject().value("@expanded").toBool())
                    this->expandItem(item);
                non_toplevel_maps.removeAt(i--); //do this at the end
            }
        }
        int cnt2 = non_toplevel_maps.count();
        if (cnt1 == cnt2)
            break; //break here if parent_id loop
    }
    this->sortItems(1,Qt::SortOrder::AscendingOrder);


    //delete items
    for (int i = 0; i < items_to_delete.count(); i++)
        delete items_to_delete.at(i); //items cant be deleted while chosen

    this->verticalScrollBar()->setValue(scroll);
}

void MapTreeWidget::clicked_at_item(QTreeWidgetItem *current_item)
{
    if (current_item->text(2).toInt() < 0) return;
    int list_id = current_item->text(2).toInt(); //id (2nd column) which it has in the list
    emit on_map_selected(list_id);
}

void MapTreeWidget::prepare_context_menu( const QPoint & pos )
{
    menu.exec(this->mapToGlobal(pos));
}

void MapTreeWidget::show_map_properties_dialog()
{
    if (this->currentItem()->text(2).toInt() < 0) return;
    if (this->selectedItems().size() <= 0)
        return;
    MapPropertiesDialog *dialog = new MapPropertiesDialog(this->db, this->mic, this->currentItem()->text(2).toInt(), 0);
    connect(dialog, SIGNAL(ok_clicked()), this, SLOT(list_maps()));
    dialog->show();
}

void MapTreeWidget::create_new_map()
{
    int current = 0;
    if (this->currentItem()->text(2).toInt() > 0)
    {
        current = this->currentItem()->text(2).toInt();
    }

    int id = this->mic->get_lowest_available_id();
    int parent = 0;
    if (current > 0)
        parent = this->mic->get_parent(current);

    MapPropertiesDialog *dialog = new MapPropertiesDialog(this->db,this->mic, id, 0);
    connect(dialog, &MapPropertiesDialog::ok_clicked, this, [=](){
        this->mic->set_parent(id, parent); //parent is set after creation of the map
        this->list_maps();
    });
    dialog->show();
}

void MapTreeWidget::copy_map()
{
    if (this->currentItem()->text(2).toInt() < 0) return;

    QJsonDocument doc(mic->get_db()->get_mapfile_by_id(this->currentItem()->text(2).toInt())->object());
    QSettings settings;
    settings.setValue("copied_map", doc.toJson(QJsonDocument::Compact));
    settings.setValue("copied_map_name", mic->get_name(this->currentItem()->text(2).toInt()));
}

void MapTreeWidget::paste_map()
{
    int current = 0;
    if (this->currentItem()->text(2).toInt() > 0)
    {
        current = this->currentItem()->text(2).toInt();
    }

    int id = this->mic->get_lowest_available_id();
    int parent = 0;
    if (current > 0)
        parent = this->mic->get_parent(current);


    QSettings settings;
    QJsonParseError err;
    QJsonObject map_object = QJsonDocument::fromJson(settings.value("copied_map").toByteArray(), &err).object();
    if (err.error != QJsonParseError::NoError) return;

    if (this->mic->create_map(id))
    {
        //when map created
        this->db->get_mapfile_by_id(id)->setObject(map_object);
        this->mic->set_name(id, settings.value("copied_map_name").toString());
        this->mic->set_parent(id,parent);
        this->list_maps();
    }
}

void MapTreeWidget::delete_map()
{
    if (this->currentItem()->text(2).toInt() < 0) return;
    int id = this->currentItem()->text(2).toInt();
    QString name = mic->get_name(id);

    int res = QMessageBox::question(this, "Deleting Map", QString("Do you really want to delete %1?").arg(name));
    if (res == QMessageBox::Button::Yes)
    {
        QJsonObject mapinfos = this->db->get_mapinfos()->object();
        mapinfos.remove(QString::number(id));
        this->db->get_mapinfos()->setObject(mapinfos);
        this->db->remove_map_file_by_id(id);
        this->list_maps();
    }
}

void MapTreeWidget::itemExpanded(QTreeWidgetItem *item)
{
    mic->set_expanded(item->text(2).toInt(), true);
}

void MapTreeWidget::itemCollapsed(QTreeWidgetItem *item)
{
    mic->set_expanded(item->text(2).toInt(), false);
}

void MapTreeWidget::dropEvent(QDropEvent *event)
{
    //allow reordering
    QTreeWidget::dropEvent(event); //do the drag and drop in super class

    this->restore_after_move();
}



