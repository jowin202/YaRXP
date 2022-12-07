#include "trooppiclabel.h"


#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

TroopPicLabel::TroopPicLabel(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);
}

void TroopPicLabel::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    connect(ec, SIGNAL(current_troop_changed()), this, SLOT(redraw()));
}

void TroopPicLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        bool clicked_at_object = false;
        this->left_clicked = true;
        for (int i = 0; i < this->members.count(); i++)
        {
            if (this->bounding_rects[i].contains(ev->pos()))
            {
                this->marked_member = i;
                clicked_at_object = true;
                this->rel_pos = ev->pos() - (this->bounding_rects[i].topLeft() + QPoint(this->bounding_rects[i].width()/2, this->bounding_rects[i].height()));
                break;
            }
        }
        if (!clicked_at_object)
            this->marked_member = -1;
        this->redraw();
    }
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

/*
void TroopPicLabel::setData(RPGSystem *system, RPGTroop *troop)
{
    this->system = system;
    this->troop = troop;
    this->marked_member = -1;

    this->clear_troop();    //Delete old troop members if exists

    //qDebug() << "";
    for (RPGTroopMember *member : troop->members)
    {
        RPGTroopMember *m = new RPGTroopMember;
        member->copy_to(m);
        this->members.append(m);
        //qDebug() << member->x << member->y;
    }
    this->redraw();
}
*/

/*
void TroopPicLabel::arrange()
{
    int n = this->members.count();
    int cnt = 0;

    for (RPGTroopMember *member : this->members)
    {
        if (n == 1)
            member->x = 320;
        else if (n == 2)
            member->x = 266 + (cnt++) * 92;
        else if (n == 3)
            member->x = 176 + (cnt++) * 120;
        else if (n == 4)
            member->x = 104 + (cnt++) * 144;
        else if (n == 5)
            member->x = 33 + (cnt++) * 55;
        else if (n == 6)
            member->x = 33 + (cnt++) * 55;
        else if (n == 7)
            member->x = 33 + (cnt++) * 55;
        else if (n == 8)
            member->x = 33 + (cnt++) * 55;

        member->y = 304;
    }

    this->redraw();
}
*/

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
    QImage battleback(ec->get_db()->project_dir + "Graphics" + QDir::separator() + "Battlebacks" + QDir::separator() + battleback_name);

    if (!battleback.isNull())
        battleback = battleback.scaled(battleback.width()/2, battleback.height()/2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QImage img(320, 160, QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&img);
    painter.fillRect(0,0,img.width(),img.height(), Qt::black);
    if (!battleback.isNull())
        painter.drawImage(QRect(0,0,battleback.width(),battleback.height()), battleback);

    for (int i = 0; i < this->members.count(); i++)
    {
        QJsonObject member = this->members.at(i).toObject();
        int x = member.value("@x").toInt();
        int y = member.value("@y").toInt();
        //qDebug() << member->enemy_id << member->x << member->y << member->hidden;
        /*
         * //TODO
        if (this->members.at(i)->enemy_id > system->enemies_list.length())
            continue;
            */


        if (member.value("@hidden").toBool()) painter.setOpacity(0.4);
        else painter.setOpacity(1);


        QJsonObject enemy = ec->get_object_by_id(RPGDB::ENEMIES,member.value("@enemy_id").toInt());
        QImage battler = QImage(ec->get_db()->project_dir + "Graphics" + QDir::separator() + "Battlers" + QDir::separator() + enemy.value("@battler_name").toString());

        if (battler.isNull()) continue;
        battler = battler.scaled(battler.width()/2, battler.height()/2);
        painter.drawImage(x/2-battler.width()/2,y/2 - battler.height(), battler);
        this->bounding_rects[i].setRect(x/2-battler.width()/2, y/2 - battler.height(),battler.width(), battler.height());

        if (this->marked_member == i)
        {
            painter.setOpacity(1);
            QPen pen = painter.pen();
            painter.setPen(Qt::white);
            painter.drawRect(this->bounding_rects[i]);
            painter.setPen(pen);
        }
    }
    //other bounding rects should be empty
    for (int i = this->members.count(); i < 8; i++)
        this->bounding_rects[i].setRect(0,0,0,0);


    painter.end();

    this->setPixmap(QPixmap::fromImage(img));
}

