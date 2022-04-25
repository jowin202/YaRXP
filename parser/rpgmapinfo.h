#ifndef RPGMAPINFO_H
#define RPGMAPINFO_H

#include <QObject>

class RPGMapInfo : public QObject
{
    Q_OBJECT
public:
    explicit RPGMapInfo(QObject *parent = nullptr);

signals:

private:
    bool expanded;
    int parent_id;
    int scroll_x;
    int scroll_y;
    int order;
    QString name;

};

#endif // RPGMAPINFO_H
