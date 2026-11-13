#ifndef UNDODB_H
#define UNDODB_H

#include <QObject>
#include <QJsonObject>
#include <QStack>
#include <QHash>

class UndoDB : public QObject
{
    Q_OBJECT
public:
    explicit UndoDB(QObject *parent = nullptr);
    void push(int map, QJsonObject obj, bool isFirst = false);
    QJsonObject pop(int map);
    void clear() { this->maps.clear(); }
    void clear(int map) { this->maps.remove(map); }

signals:

private:
    QHash<int,QStack<QJsonObject>> maps;
    static const int max_steps = 100; //per map, every step is a full copy of the map

};

#endif // UNDODB_H
