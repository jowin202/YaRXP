#include "trooppiclabel.h"


#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"
#include "RXIO2/rpgeditorcontroller.h"

TroopPicLabel::TroopPicLabel(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(open_context_menu(QPoint)));

    this->halfway = new QAction("Appear halfway");
    this->halfway->setCheckable(true);
    this->immortal = new QAction("Immortal");
    this->immortal->setCheckable(true);

    connect(this->halfway, SIGNAL(triggered()), this, SLOT(toggle_halfway()));
    connect(this->immortal, SIGNAL(triggered()), this, SLOT(toggle_immortal()));
}

void TroopPicLabel::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    connect(ec, &RPGEditorController::current_troop_changed, this, [=]() { this->marked_member = -1; }); //no marked element when change the troop
    connect(ec, SIGNAL(current_troop_changed()), this, SLOT(redraw()));
}

void TroopPicLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        this->left_clicked = true; //for moving
    }


    bool clicked_at_object = false;
    for (int i = 0; i < this->members.count(); i++)
    {
        if (this->bounding_rects[i].contains(ev->pos()))
        {
            this->marked_member = i;
            clicked_at_object = true;
            this->rel_pos = ev->pos() - (QPoint(this->bounding_rects[i].topLeft().x(),this->bounding_rects[i].topLeft().y()) + QPoint(this->bounding_rects[i].width()/2, this->bounding_rects[i].height()));
            break;
        }
    }
    if (!clicked_at_object)
        this->marked_member = -1;
    this->redraw();
}

void TroopPicLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
        this->left_clicked = false;
}

void TroopPicLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if (ev->position().x() <= 0 || ev->position().y() <= 0 || ev->position().x() >= 320 || ev->position().y() >= 160)
        return; //out of range

    if (this->left_clicked && this->marked_member >= 0)
    {
        QPoint new_pos = 2*ev->pos() - 2*this->rel_pos;
        QJsonObject tmp_member = this->members.at(marked_member).toObject();
        tmp_member.insert("@x", new_pos.x());
        tmp_member.insert("@y", new_pos.y());
        this->members.removeAt(marked_member);
        this->members.insert(marked_member,tmp_member);

        ec->obj_set_jsonvalue(RPGDB::TROOPS, "@members", this->members);

        this->redraw();
    }
}

void TroopPicLabel::remove_current()
{
    if (marked_member >= 0 && marked_member < this->members.count())
    {
        QJsonArray tmp_members = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray();
        tmp_members.removeAt(marked_member);
        ec->obj_set_jsonvalue(RPGDB::TROOPS, "@members", tmp_members);
        this->marked_member = -1;
        this->redraw();
    }
}



void TroopPicLabel::redraw()
{
    members = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray();
    QString battleback_name = ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battleback_name").toString();
    QImage battleback = FileOpener(ec->get_db()->battleback_dir,battleback_name).get_image();

    if (!battleback.isNull())
        battleback = battleback.scaled(battleback.width()/2, battleback.height()/2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QImage img(320, 160, QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&img);
    painter.fillRect(0,0,img.width(),img.height(), Qt::black);
    if (!battleback.isNull())
        painter.drawImage(QRect(0,0,battleback.width(),battleback.height()), battleback);


    //set all bounding rects to zero should be empty
    for (int i = 0; i < 8; i++)
        this->bounding_rects[i].setRect(0,0,0,0);

    for (int i = 0; i < this->members.count(); i++)
    {
        QJsonObject member = this->members.at(i).toObject();
        int x = member.value("@x").toInt();
        int y = member.value("@y").toInt();

        if (member.value("@hidden").toBool()) painter.setOpacity(0.4);
        else painter.setOpacity(1);


        QJsonObject enemy = ec->get_object_by_id(RPGDB::ENEMIES,member.value("@enemy_id").toInt());
        QImage battler = FileOpener(ec->get_db()->battler_dir,enemy.value("@battler_name").toString()).get_image();

        if (battler.isNull()) continue;
        battler = battler.scaled(battler.width()/2, battler.height()/2);

        //adjust hue
        battler.convertTo(QImage::Format_ARGB32_Premultiplied);
        int hue = enemy.value("@battler_hue").toInt();
        for (int y = 0; y < battler.height(); y++)
        {
            for (int x = 0; x < battler.width(); x++)
            {
                QColor col = battler.pixelColor(x,y);
                if (col.alpha() == 0) continue;
                col.setHsv(col.hsvHue() + hue,col.saturation(), col.value());
                battler.setPixelColor(x,y,col);
            }
        }

        painter.drawImage(x/2-battler.width()/2,y/2 - battler.height(), battler);
        this->bounding_rects[i].setRect(x/2.0-battler.width()/2.0, y/2.0 - battler.height(),battler.width(), battler.height());

        if (this->marked_member == i)// || true) //TODO: delete
        {
            painter.setOpacity(1);
            QPen pen = painter.pen();
            painter.setPen(Qt::white);
            painter.drawRect(this->bounding_rects[i]);
            painter.setPen(pen);
        }
    }

    painter.end();
    //this->arrange(); //TODO: delete
    this->setPixmap(QPixmap::fromImage(img));
}

void TroopPicLabel::open_context_menu(QPoint pos)
{
    if (marked_member < 0 || marked_member > 8) return;

    QMenu menu;
    halfway->setChecked(this->members.at(marked_member).toObject().value("@hidden").toBool());
    immortal->setChecked(this->members.at(marked_member).toObject().value("@immortal").toBool());
    menu.addAction(halfway);
    menu.addAction(immortal);

    menu.exec(this->mapToGlobal(pos));
}

void TroopPicLabel::toggle_halfway()
{
    if (marked_member < 0 || marked_member > 8) return;
    QJsonObject tmp_member = this->members.at(marked_member).toObject();
    tmp_member.insert("@hidden", !tmp_member.value("@hidden").toBool());
    this->members.removeAt(marked_member);
    this->members.insert(marked_member,tmp_member);

    ec->obj_set_jsonvalue(RPGDB::TROOPS, "@members", this->members);
    this->redraw();
}

void TroopPicLabel::toggle_immortal()
{
    if (marked_member < 0 || marked_member > 8) return;
    QJsonObject tmp_member = this->members.at(marked_member).toObject();
    tmp_member.insert("@immortal", !tmp_member.value("@immortal").toBool());
    this->members.removeAt(marked_member);
    this->members.insert(marked_member,tmp_member);

    ec->obj_set_jsonvalue(RPGDB::TROOPS, "@members", this->members);
    this->redraw();
}

void TroopPicLabel::arrange()
{
    QJsonArray members = this->ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray();
    QCryptographicHash hash1(QCryptographicHash::Sha3_256);
    QCryptographicHash hash2(QCryptographicHash::Sha3_256);
    hash1.addData(QJsonDocument(members).toJson(QJsonDocument::Compact));

    int sum = 0;
    int member_count = members.count();
    QList<int> members_x;
    for (int i = 0; i < members.count(); i++)
    {
        members_x.append(members.at(i).toObject().value("@x").toInt());
        sum += bounding_rects[i].width();
    }


    if (member_count == 0)
        return;
    else if (member_count == 1)
    {
        QJsonObject member = members.at(0).toObject();
        member.insert("@x", 320);
        member.insert("@y", 304);
        members.removeAt(0);
        members.insert(0,member);
    }
    else
    {
        qDebug() << members;
        //qDebug() << members_x;
        qDebug() << bounding_rects[0].width() << bounding_rects[1].width() << bounding_rects[2].width() << bounding_rects[3].width();

        qreal spacing = 16;
        qreal border_space = (640-2*sum - 16*(member_count-1))/2.0;
        if (border_space < 8)
        {
            border_space = 8;
            spacing = (640-2*sum-8-6)/(member_count-1);
        }
        qreal x = border_space + bounding_rects[0].width();

        qDebug() << "border_space: " << border_space << member_count;
        qDebug() << "orig:" << spacing << "calc: " << members_x[1]-members_x[0] - bounding_rects[1].width() - bounding_rects[0].width();
        QJsonObject member;
        for (int i = 0; i < member_count; i++)
        {
            if (i>0)
                x += bounding_rects[i-1].width() + bounding_rects[i].width() + spacing;
            member = members.at(i).toObject();
            member.insert("@x", x);
            member.insert("@y", 304);
            members.removeAt(i);
            members.insert(i,member);
        }
    }

    this->ec->obj_set_jsonvalue(RPGDB::TROOPS, "@members", members);

    hash2.addData(QJsonDocument(members).toJson(QJsonDocument::Compact));
    qDebug() << (hash1.result() == hash2.result()) << hash1.result().toHex() << hash2.result().toHex();

}

