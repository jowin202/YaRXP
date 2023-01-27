#ifndef IMAGEDISPLAYWIDGET_H
#define IMAGEDISPLAYWIDGET_H

#include <QObject>
#include <QLabel>

#include <QMouseEvent>

class RPGDB;
class ImageDisplayWidget : public QLabel
{
    Q_OBJECT
public:
    ImageDisplayWidget(QWidget *parent);

    void set_background_color();

    void update_image();
    void set_data(RPGDB *db, int mode, QString current_file);
    void set_data(RPGDB *db, int mode, QString current_file, int hue);
    void set_data_from_page(RPGDB *db, QString character_name, int character_hue, int pattern, int direction, int opacity, int blend_type, int tile_id, int tileset_id);
    QJsonObject get_data_from_page();



    void mouseDoubleClickEvent(QMouseEvent *ev);

    QString current_file;
    int hue = 0;


signals:
    void image_changed();
    void hue_changed();

public slots:
    void set_current(QString file)
    { this->current_file = file; this->update_image(); emit image_changed(); }
    void set_hue(int hue)
    { this->hue = hue; this->update_image(); emit hue_changed(); }

private:
    RPGDB *db;
    int mode;

    int pattern;
    int direction;
    int opacity;
    int blend_type;
    int tile_id;
    int tileset_id;

};

#endif // IMAGEDISPLAYWIDGET_H
