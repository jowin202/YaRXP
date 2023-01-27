#ifndef IMAGECHOOSERDIALOG_H
#define IMAGECHOOSERDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QPen>
#include <QPainter>
#include <QGraphicsView>

class RPGDB;


class Rectangle : public QGraphicsItem
{
public:
    Rectangle(int w, int h)
    {
        this->w = w;
        this->h = h;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override {
        QPen pen;
        pen.setColor(QColor(200,200,200));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(1,1,w-1,h-1);
        pen.setColor(QColor(150,150,150));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(2,2,w-2,h-2);
        Q_UNUSED(option);
        Q_UNUSED(widget);
    };
    QRectF boundingRect() const override { return QRect(0,0,w,h); };

    int w = 32;
    int h = 32;
};

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CustomGraphicsView(QWidget *parent) : QGraphicsView(parent)
    {
    }

    void mousePressEvent(QMouseEvent *e)
    {
        QPointF pos = mapToScene(QPoint(e->pos().x(), e->pos().y()));

        if (rectangle != 0)
        {
            this->rectangle->setPos(((int)pos.x()/rectangle->w)*rectangle->w, ((int)pos.y()/rectangle->h)*rectangle->h);
            emit clicked(pos.x()/rectangle->w, pos.y()/rectangle->h);
        }
    }
    Rectangle *rectangle = 0;

signals:
    void clicked(int,int);
};

namespace Ui {
class ImageChooserDialog;
}

class ImageChooserDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ImageChooserDialog(RPGDB *db, QString character_name, int character_hue, int pattern,
                                int direction, int opacity, int blend_type, int tile_id, int tileset_id,
                                QWidget *parent = nullptr);
    ~ImageChooserDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked(QString, int, int, int, int, int, int);
private slots:
    void update_image_list();
    void update_image();

    void on_button_ok_clicked();
    void on_button_cancel_clicked();

    void on_list_currentRowChanged(int currentRow);

    void on_slider_hue_sliderMoved(int position);

private:
    Ui::ImageChooserDialog *ui;
    RPGDB *db;
    QString character_name;
    int pattern;
    int direction;
    int tile_id;
    int tileset_id;
};

#endif // IMAGECHOOSERDIALOG_H
