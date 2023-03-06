#ifndef UNDODB_H
#define UNDODB_H

#include <QObject>
#include <QJsonObject>
#include <QStack>

class UndoDB : public QObject
{
    Q_OBJECT
public:
    explicit UndoDB(QObject *parent = nullptr);
    void push(int map, QJsonObject obj);
    QJsonObject pop(int map);

signals:

private:
    QStack<QJsonObject> maps[999];

};

#endif // UNDODB_H
