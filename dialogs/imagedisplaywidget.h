#ifndef IMAGEDISPLAYWIDGET_H
#define IMAGEDISPLAYWIDGET_H

#include <QObject>
#include <QLabel>

#include <QMouseEvent>

class RPGSystem;
class RPGEventPage;
class ImageDisplayWidget : public QLabel
{
    Q_OBJECT
public:
    ImageDisplayWidget(QWidget *parent);

    void set_background_color();

    void update_image();
    void set_data(RPGSystem *system, int mode, QString current_file);
    void set_data(RPGSystem *system, int mode, QString current_file, int hue);
    void set_data_from_page(RPGSystem *system, RPGEventPage *page);

    void mouseDoubleClickEvent(QMouseEvent *ev);

    QString current_file;
    int hue = 0;
    int tile_id;

public slots:
    void set_current(QString file)
    { this->current_file = file; this->update_image(); }
    void set_hue(int hue)
    { this->hue = hue; this->update_image(); }

private:
    RPGSystem *system;
    int mode;
    RPGEventPage *page;

};

#endif // IMAGEDISPLAYWIDGET_H
