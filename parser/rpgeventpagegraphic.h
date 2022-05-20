#ifndef RPGEVENTPAGEGRAPHIC_H
#define RPGEVENTPAGEGRAPHIC_H

#include <QObject>
#include <QImage>

class RPGEventPageGraphic : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventPageGraphic(QObject *parent = nullptr);

    int direction;
    int blend_type;
    int tile_id;
    int pattern;
    int character_hue;
    int opacity;
    QString character_name;

    QImage graphics;

signals:

};

#endif // RPGEVENTPAGEGRAPHIC_H
