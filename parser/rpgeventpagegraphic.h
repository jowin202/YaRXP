#ifndef RPGEVENTPAGEGRAPHIC_H
#define RPGEVENTPAGEGRAPHIC_H

#include <QObject>

class RPGEventPageGraphic : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventPageGraphic(QObject *parent = nullptr);

signals:

private:
    int direction;
    int blend_type;
    int tile_id;
    int pattern;
    QString character_name; // file name in character folder
    int character_hue;
    int opacity;

};

#endif // RPGEVENTPAGEGRAPHIC_H
