#ifndef RPGCOMMONEVENT_H
#define RPGCOMMONEVENT_H

#include <QObject>

class RPGCommonEvent : public QObject
{
    Q_OBJECT
public:
    explicit RPGCommonEvent(QObject *parent = nullptr);

signals:

};

#endif // RPGCOMMONEVENT_H
