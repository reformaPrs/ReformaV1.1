#include "pointer.h"

Pointer::Pointer(QWidget *parent):
    QLabel(parent)
{

}

void Pointer::mousePressEvent(QMouseEvent *event)
{
    emit pressed(event->pos());
}

void Pointer::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit released();
}
