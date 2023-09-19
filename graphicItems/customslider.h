#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QMouseEvent>
#include <QSlider>


/*!
 * \brief Класс слайдера с возможностью нажатия на шкалу
 */
class CustomSlider: public QSlider
{

public:
    CustomSlider(QWidget *parent = nullptr);
protected:
    virtual void paintEvent(QPaintEvent* event) override;
    /*!
   * \brief mousePressEvent вызывается при нажатии на слайдер
   * \param event
   *
   * Перемещает бегунок на место нажатия
   */
    void mousePressEvent ( QMouseEvent * event )
    {
        if (event->button() == Qt::LeftButton)
        {
            if (orientation() == Qt::Vertical) {
                setValue(minimum() + ((maximum()-minimum() + 1) * (height()-event->pos().y())) / height() ) ;
            }
            else {
                setValue(minimum() + ((maximum()-minimum() + 1) * (event->pos().x())) / width() ) ;
            }
            event->accept();
        }
        QSlider::mousePressEvent(event);
    }
};

#endif // CUSTOMSLIDER_H
