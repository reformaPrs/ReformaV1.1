#ifndef HOVERPIXMAPITEM_H
#define HOVERPIXMAPITEM_H

#include <QGraphicsPixmapItem>

/*!
 * \brief Класс картинки с сигналами при нажатии наведении и выхода из наведения
 */
class HoverPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    /*!
     * \brief HoverPixmapItem
     * \param pixmap - картинка объекта
     * \param parent
     */
    HoverPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
signals:
    /*!
     * \brief mouseClicked вызывается при нажатии на картинку
     */
    void mouseClicked();
    /*!
     * \brief hoverEntered вызывается при наведении на картинку
     */
    void hoverEntered();
    /*!
     * \brief hoverLeft вызывается при выходе из наведения на картинку
     */
    void hoverLeft();
    // QGraphicsItem interface
protected:
    /*!
     * \brief hoverEnterEvent ловит событие наведения на картинку
     * \param event
     *
     * Вызывает hoverEntered()
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    /*!
     * \brief hoverLeaveEvent ловит событие выхода из наведения на картинку
     * \param event
     *
     * Вызывает hoverLeft()
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    /*!
     * \brief mousePressEvent ловит собызия нажатия на картинку
     * \param event
     *
     * Вызывает mouseClicked()
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // HOVERPIXMAPITEM_H
