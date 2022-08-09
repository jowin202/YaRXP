#ifndef RPGTONE_H
#define RPGTONE_H

#include <QObject>

class RPGTone : public QObject
{
    Q_OBJECT
public:
    explicit RPGTone(QObject *parent = nullptr);
    double red;
    double green;
    double blue;
    double gray;

signals:

};

#endif // RPGTONE_H
