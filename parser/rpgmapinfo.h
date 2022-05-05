#ifndef RPGMAPINFO_H
#define RPGMAPINFO_H

#include <QDebug>
#include <QObject>

class RPGMapInfo : public QObject
{
    Q_OBJECT
public:
    explicit RPGMapInfo(QObject *parent = nullptr);
    void set_id(int id);
    void setParameter(QString param, int val);
    void setParameter(QString param, bool val);
    void setParameter(QString param, QString val);

    void debug();

    int id;
    bool expanded;
    int parent_id;
    int scroll_x;
    int scroll_y;
    int order;
    QString name;

signals:

private:


};

#endif // RPGMAPINFO_H
