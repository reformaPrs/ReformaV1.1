#ifndef DOT_H
#define DOT_H

#include <QGraphicsEllipseItem>
#include <QObject>

/*!
 * \brief Класс точки
 *
 * Точка, с помощью которой можно отслеживать нажатие и наведение на неё.
 * Также имеет координаты в центре круга, а не в углу.
 */
class Dot : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    /*!
     * \brief Dot
     * \param x - координата
     * \param y - координата
     * \param r - радиус
     */
    Dot(int x, int y, int r, QGraphicsItem *parent = nullptr);

    /*!
     * \brief setRadius задаёт новый радиус
     * \param r
     *
     * В резултате точка перерисована с центром в той же точке, что и раньше
     */
    void setRadius(int r);

    void setOffset(QPoint offset);

signals:
    /*!
     * \brief hoverEntered вызывается при наведении на точку
     */
    void hoverEntered();
    /*!
     * \brief hoverLeft вызывается при выходе из наведения на точку
     */
    void hoverLeft();
    /*!
     * \brief clicked вызывается при нажатии на точку
     */
    void clicked();

    // QGraphicsItem interface
protected:
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // DOT_H
