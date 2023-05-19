#ifndef CONNECTEDMAPVIEW_H
#define CONNECTEDMAPVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class ConnectedMapView : public QGraphicsView
{
    Q_OBJECT

public:
    ConnectedMapView(QWidget *parent);
    void wheelEvent(QWheelEvent *event);

signals:
    void zoom_in();
    void zoom_out();

};

#endif // CONNECTEDMAPVIEW_H
