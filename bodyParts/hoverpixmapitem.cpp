#include "hoverpixmapitem.h"

#include <QGraphicsSceneMouseEvent>

HoverPixmapItem::HoverPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent):
    QGraphicsPixmapItem(pixmap, parent)
{

}

void HoverPixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverEntered();
    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void HoverPixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverLeft();
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

void HoverPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit mouseClicked();
    QGraphicsPixmapItem::mousePressEvent(event);
}
