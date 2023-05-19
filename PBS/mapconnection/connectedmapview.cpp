#include "connectedmapview.h"

ConnectedMapView::ConnectedMapView(QWidget *parent) : QGraphicsView(parent)
{

}

void ConnectedMapView::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::Modifier::CTRL) != 0)
    {
        if (event->angleDelta().y() > 0)
            emit zoom_in();
        else emit zoom_out();

        return;
    }
    event->accept();
    QGraphicsView::wheelEvent(event);
}
