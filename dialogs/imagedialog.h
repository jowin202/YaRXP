#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QWidget>
#include <QDir>
#include <QDirIterator>
#include <QKeyEvent>
#include <QListWidgetItem>

namespace Ui {
class ImageDialog;
}

class RPGDB;

class ImageDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ImageDialog(RPGDB *db, int mode, QString current_image_file, QWidget *parent = nullptr);
    ~ImageDialog();

    void set_hue(int hue);
    void set_fog_options(int fog_sx, int fog_sy, int fog_opacity, int fog_zoom, int fog_blend_type);

    enum { EVENTPAGE, ANIMATIONS, AUTOTILES, BATTLEBACKS, BATTLERS, CHARACTERS, FOGS, ICONS, PANOMRAMAS, PICTURES, TILESETS, TITLES, TRANSITIONS, WINDOWSKINS, GAMEOVERS};

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked(QString);
    void ok_clicked_with_hue(int);
    void ok_clicked_with_fog_options(int,int,int,int,int);

public slots:
    void update_image_list();
    void update_image();

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

    void on_list_currentRowChanged(int currentRow);

    void on_slider_hue_valueChanged(int value);

    void on_list_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::ImageDialog *ui;
    RPGDB *db;
    int mode;
    QString current;
    QString folder_name;

    QImage current_img;
    int hue = 0;


    bool visible_hue = false;
    bool visible_options = false;
    bool visible_fog_options = false;
};

#endif // IMAGEDIALOG_H
