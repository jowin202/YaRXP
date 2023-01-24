#include "shopprocessingdialog.h"
#include "ui_shopprocessingdialog.h"

#include "shopgoodsdialog.h"
#include "RXIO2/rpgdb.h"

ShopProcessingDialog::ShopProcessingDialog(RPGDB *db, QJsonArray shop_params, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopProcessingDialog)
{
    ui->setupUi(this);
    this->db = db;
    this->shop_params = shop_params;


    action_delete.setText("Delete");
    action_edit.setText("Edit");
    action_item.setText("New Item");
    action_weapon.setText("New Weapon");
    action_armor.setText("New Armor");


    action_delete.setShortcut(Qt::Key_Delete);
    action_delete.setShortcutContext(Qt::WidgetShortcut);
    this->ui->tableWidget->addAction(&action_delete);

    connect(&action_delete, SIGNAL(triggered()), this, SLOT(del()));
    connect(&action_edit, SIGNAL(triggered()), this, SLOT(edit()));
    connect(&action_item, SIGNAL(triggered()), this, SLOT(new_item()));
    connect(&action_weapon, SIGNAL(triggered()), this, SLOT(new_weapon()));
    connect(&action_armor, SIGNAL(triggered()), this, SLOT(new_armor()));

    connect(this->ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_customContextMenuRequested(const QPoint&)));
    connect(this->ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(edit()));

    this->update_table();
}

ShopProcessingDialog::~ShopProcessingDialog()
{
    delete ui;
}

void ShopProcessingDialog::on_button_ok_clicked()
{
    emit ok_clicked(this->shop_params);
    this->close();
}


void ShopProcessingDialog::on_button_cancel_clicked()
{
    this->close();
}

void ShopProcessingDialog::update_table()
{
    this->ui->tableWidget->clearContents();
    QTableWidgetItem *item, *item2;
    this->ui->tableWidget->setRowCount(shop_params.count());
    for (int i = 0; i < shop_params.count(); i++)
    {
        if (shop_params.at(i).toArray().at(0) == 0)
        {
            this->ui->tableWidget->setItem(i,0, item = new QTableWidgetItem(db->get_object_name(RPGDB::ITEMS, shop_params.at(i).toArray().at(1).toInt())));
            this->ui->tableWidget->setItem(i,1, item2 = new QTableWidgetItem(QString::number(db->get_object_price(RPGDB::ITEMS, shop_params.at(i).toArray().at(1).toInt()))));
        }
        else if (shop_params.at(i).toArray().at(0) == 1)
        {
            this->ui->tableWidget->setItem(i,0, item = new QTableWidgetItem(db->get_object_name(RPGDB::WEAPONS, shop_params.at(i).toArray().at(1).toInt())));
            this->ui->tableWidget->setItem(i,1, item2 = new QTableWidgetItem(QString::number(db->get_object_price(RPGDB::WEAPONS, shop_params.at(i).toArray().at(1).toInt()))));
        }
        else //if (shop_params.at(i).toArray().at(0) == 2)
        {
            this->ui->tableWidget->setItem(i,0, item = new QTableWidgetItem(db->get_object_name(RPGDB::ARMORS, shop_params.at(i).toArray().at(1).toInt())));
            this->ui->tableWidget->setItem(i,1, item2 = new QTableWidgetItem(QString::number(db->get_object_price(RPGDB::ARMORS, shop_params.at(i).toArray().at(1).toInt()))));
        }
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
    }


}

void ShopProcessingDialog::new_item()
{
    ShopGoodsDialog *dialog = new ShopGoodsDialog(db,0,1);
    dialog->show();
    connect(dialog, &ShopGoodsDialog::ok_clicked, [=](int type, int value){
        QJsonArray p;
        p.append(type);
        p.append(value);
        this->shop_params.insert(this->ui->tableWidget->rowCount(),p);
        this->update_table();
    });
}

void ShopProcessingDialog::new_weapon()
{
    ShopGoodsDialog *dialog = new ShopGoodsDialog(db,1,1);
    dialog->show();
    connect(dialog, &ShopGoodsDialog::ok_clicked, [=](int type, int value){
        QJsonArray p;
        p.append(type);
        p.append(value);
        this->shop_params.insert(this->ui->tableWidget->rowCount(),p);
        this->update_table();
    });
}

void ShopProcessingDialog::new_armor()
{
    ShopGoodsDialog *dialog = new ShopGoodsDialog(db,2,1);
    dialog->show();
    connect(dialog, &ShopGoodsDialog::ok_clicked, [=](int type, int value){
        QJsonArray p;
        p.append(type);
        p.append(value);
        this->shop_params.insert(this->ui->tableWidget->rowCount(),p);
        this->update_table();
    });
}

void ShopProcessingDialog::edit()
{
    ShopGoodsDialog *dialog = new ShopGoodsDialog(db,this->shop_params.at(this->ui->tableWidget->currentRow()).toArray().at(0).toInt(),
                                                  this->shop_params.at(this->ui->tableWidget->currentRow()).toArray().at(1).toInt());
    dialog->show();
    connect(dialog, &ShopGoodsDialog::ok_clicked, [=](int type, int value){
        QJsonArray p;
        p.append(type);
        p.append(value);
        this->shop_params.removeAt(this->ui->tableWidget->currentRow());
        this->shop_params.insert(this->ui->tableWidget->currentRow(),p);
        this->update_table();
    });
}

void ShopProcessingDialog::del()
{
    this->shop_params.removeAt(this->ui->tableWidget->selectedItems().at(0)->row());
    this->update_table();
}

void ShopProcessingDialog::on_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *item = this->ui->tableWidget->itemAt(pos);
    if (item) {
        QMenu menu;
        menu.addAction(&action_edit);
        menu.addAction(&action_delete);
        menu.addSeparator();
        menu.addAction(&action_item);
        menu.addAction(&action_weapon);
        menu.addAction(&action_armor);

        menu.exec(this->mapToGlobal(pos));
    }
}


void ShopProcessingDialog::on_button_item_clicked()
{
    this->new_item();
}


void ShopProcessingDialog::on_button_weapon_clicked()
{
    this->new_weapon();
}


void ShopProcessingDialog::on_button_armor_clicked()
{
    this->new_armor();
}


void ShopProcessingDialog::on_button_del_clicked()
{
    this->del();
}

