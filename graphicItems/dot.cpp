#include "dot.h"

Dot::Dot(int x, int y, int r, QGraphicsItem *parent):
    QGraphicsEllipseItem(x - r, y - r, 2 * r, 2 * r, parent)
{

}

void Dot::setRadius(int r)
{
    int h = rect().height();
    int w = rect().width();
    int x = rect().x() + w / 2 - r;
    int y = rect().y() + h / 2 - r;
    setRect(x, y, 2*r, 2*r);
}

void Dot::setOffset(QPoint offset)
{
    int h = rect().height();
    int w = rect().width();
    int x = rect().x() + offset.x();
    int y = rect().y() + offset.y();
    setRect(x, y, w, h);
}

void Dot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    emit hoverLeft();
}

void Dot::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    emit hoverEntered();
}

void Dot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    emit clicked();
}
