#ifndef POINTER_H
#define POINTER_H

#include <QLabel>
#include <QObject>
#include <QMouseEvent>
/*!
 * \brief Класс указателя
 *
 */
class Pointer : public QLabel
{
    Q_OBJECT
public:
    Pointer(QWidget *parent = nullptr);

signals:
    /*!
     * \brief pressed вызывается при удерживании мышью указателя
     */
    void pressed(QPoint);
    /*!
     * \brief released вызывается при отпускании указателя
     */
    void released();

    // QWidget interfact
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // POINTER_H
