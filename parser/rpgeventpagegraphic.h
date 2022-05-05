#ifndef RPGEVENTPAGEGRAPHIC_H
#define RPGEVENTPAGEGRAPHIC_H

#include <QObject>

class RPGEventPageGraphic : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventPageGraphic(QObject *parent = nullptr);
    void setParameter(QString param, int val);
    void setParameter(QString param, QString val);

    int direction;
    int blend_type;
    int tile_id;
    int pattern;
    int character_hue;
    int opacity;
    QString character_name;

signals:

};

#endif // RPGEVENTPAGEGRAPHIC_H
