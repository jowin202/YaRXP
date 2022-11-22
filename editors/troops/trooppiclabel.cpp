#include "trooppiclabel.h"

#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgtroop.h"

TroopPicLabel::TroopPicLabel(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);
}

void TroopPicLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        bool clicked_at_object = false;
        this->left_clicked = true;
        for (int i = 0; i < this->members.length(); i++)
        {
            RPGTroopMember *member = this->members.at(i);
            if (member->bounding_rect.contains(ev->pos()))
            {
                this->marked_member = i;
                clicked_at_object = true;
                this->rel_pos = ev->pos() - (member->bounding_rect.topLeft() + QPoint(member->bounding_rect.width()/2, member->bounding_rect.height()));
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
        this->members.at(this->members.at(marked_member)->x = new_pos.x());
        this->members.at(this->members.at(marked_member)->y = new_pos.y());

        this->redraw();
    }
}

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

void TroopPicLabel::arrange()
{
    int n = this->members.length();
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

void TroopPicLabel::clear_troop()
{
    for (RPGTroopMember *member : this->members)
        delete member;
    this->members.clear();
}

void TroopPicLabel::remove_current()
{
    if (marked_member >= 0 && marked_member < this->members.length())
    {
        delete this->members.at(marked_member);
        this->members.removeAt(marked_member);
        this->marked_member = -1;
        this->redraw();
    }
}

void TroopPicLabel::add_member(int enemy_id)
{
    if (this->members.length() >= 8)
        return; //max 8

    if (enemy_id >= 0 && enemy_id <= system->enemies_list.length())
    {
        RPGTroopMember *member = new RPGTroopMember;
        member->x = 320;
        member->y = 304;
        member->enemy_id = enemy_id;
        member->hidden = false;
        member->immortal = false;

        this->members.append(member);
        this->redraw();
    }
}

int TroopPicLabel::get_member_count()
{
    return this->members.length();
}

QStringList TroopPicLabel::get_enemies_list()
{
    QStringList result;

    for (int i = 0; i < this->members.length(); i++)
    {
        result << system->datasource.get_obj_name_by_id(this->members.at(i)->enemy_id, RPGSystem::ENEMIES, false, -1, false);
    }

    return result;
}

void TroopPicLabel::redraw()
{
    QImage battleback(system->graphics_dir + "Battlebacks" + QDir::separator() + system->battleback_name);
    battleback = battleback.scaled(battleback.width()/2, battleback.height()/2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QImage img(320, 160, QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&img);
    painter.fillRect(0,0,img.width(),img.height(), Qt::black);
    if (!battleback.isNull())
        painter.drawImage(QRect(0,0,battleback.width(),battleback.height()), battleback);

    for (int i = 0; i < this->members.length(); i++)
    {
        RPGTroopMember *member = this->members.at(i);
        //qDebug() << member->enemy_id << member->x << member->y << member->hidden;
        if (this->members.at(i)->enemy_id > system->enemies_list.length())
            continue;

        if (member->hidden) painter.setOpacity(0.4);
        else painter.setOpacity(1);

        RPGEnemy *enemy = system->enemies_list.at(this->members.at(i)->enemy_id-1);
        QImage battler = enemy->get_battler_graphics(system);
        if (battler.isNull()) continue;
        battler = battler.scaled(battler.width()/2, battler.height()/2);
        painter.drawImage(member->x/2-battler.width()/2,member->y/2 - battler.height(), battler);
        member->bounding_rect.setRect(member->x/2-battler.width()/2,member->y/2 - battler.height(),battler.width(), battler.height());

        if (this->marked_member == i)
        {
            painter.setOpacity(1);
            QPen pen = painter.pen();
            painter.setPen(Qt::white);
            painter.drawRect(member->bounding_rect);
            painter.setPen(pen);
        }


    }

    painter.end();

    this->setPixmap(QPixmap::fromImage(img));
}
