#include "resizablegraphicsview.h"


ResizableGraphicsView::ResizableGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{

}

ResizableGraphicsView::ResizableGraphicsView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent)
{

}

void ResizableGraphicsView::resizeEvent(QResizeEvent *event)
{
    if (scene() != nullptr){
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    QGraphicsView::resizeEvent(event);
}

void ResizableGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (scene() != nullptr){
        int x = event->pos().x();
        int y = event->pos().y();
        int x1 = mapFromScene(QPointF(0,0)).x();
        int y1 = mapFromScene(QPointF(0,0)).y();
        int x2 = mapFromScene(scene()->sceneRect().bottomRight()).x();
        int y2 = mapFromScene(scene()->sceneRect().bottomRight()).y();
        if (x1 > x || y1 > y || x > x2 || y > y2){
            emit clicked();
        }
    }
    QGraphicsView::mousePressEvent(event);
}
