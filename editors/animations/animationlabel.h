#ifndef ANIMATIONLABEL_H
#define ANIMATIONLABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMap>
#include <QMenu>
#include <QSettings>

class RPGEditorController;

class AnimationLabel : public QLabel
{
    Q_OBJECT
public:
    AnimationLabel(QWidget *parent);
    void setEC(RPGEditorController *ec) {this->ec = ec;}

    void update(int frame);
    void set_animation_graphic(QString name, int hue);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void mouseDoubleClickEvent(QMouseEvent *e);


    void set_max_pattern(int m) {this->max_pattern = m;}

public slots:
    void set_pattern(int p) { this->pattern = p;}
    void open_cell_properties_dialog();

    void prepare_context_menu(QPoint pos)
    {
        int x = (pos.x()-4)/4 * 8 - 320;
        int y = (pos.y()-36)/4 * 8 - 160;
        this->context_menu_pos = QPoint(x,y);
        QMenu menu;
        if (current_rectangle < 0)
        {
            menu.addAction(&this->action_new);
            menu.addAction(&this->action_paste_menu);
        }
        else
        {
            menu.addAction(&this->action_properties);
            menu.addSeparator();
            menu.addAction(&this->action_cut);
            menu.addAction(&this->action_copy);
            menu.addAction(&this->action_paste_menu);
            menu.addAction(&this->action_delete);
        }
        menu.exec(this->mapToGlobal(pos));
    }
    void paste_cell(int pattern, int x, int y, int zoom, int angle, int flip, int opacity,int blending);
    void do_copy();
    void do_paste();
    void do_paste_from_menu();
    void do_cut();
    void do_delete();

    void create_new_cell();

private:
    int current_frame = -1;
    RPGEditorController * ec = 0;

    QImage animation_graphic;
    QString current_animation_graphic;
    int current_hue = 0;

    QMap<int,QRect> current_red_rectangles;
    int current_rectangle = -1;
    bool is_moving = false;

    int x=0;//internal use only
    int y=0;

    int pattern = 0;

    int max_pattern;

    QAction action_new;
    QAction action_properties;
    QAction action_cut;
    QAction action_copy;
    QAction action_paste;
    QAction action_paste_menu;
    QAction action_delete;

    QPoint context_menu_pos;
};

#endif // ANIMATIONLABEL_H
