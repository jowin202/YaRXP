#ifndef RPGMAPINFOCONTROLLER_H
#define RPGMAPINFOCONTROLLER_H

#include <QObject>
#include <QDebug>

class RPGDB;

class RPGMapInfoController : public QObject
{
    Q_OBJECT
public:
    explicit RPGMapInfoController(RPGDB *db);

    bool id_is_valid(int id);
    int get_lowest_available_id();

    void set_expanded(int id, bool expanded);
    bool get_expanded(int id);

    void set_name(int id, QString name);
    QString get_name(int id);

    void set_order(int id, int order);
    int get_order(int id);

    void set_parent(int id, int parent);
    int get_parent(int id);

    QJsonArray get_toplevel_maps();
    QJsonArray get_child_maps();

    RPGDB *get_db() {return this->db; }

    bool create_map(int id);
    bool create_map_from_object(int id, QJsonObject map);


signals:

private:
    RPGDB *db;

};

#endif // RPGMAPINFOCONTROLLER_H
