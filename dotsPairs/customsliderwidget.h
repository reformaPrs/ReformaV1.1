#ifndef CUSTOMSLIDERWIDGET_H
#define CUSTOMSLIDERWIDGET_H

#include <QWidget>
#include <QPainterPath>
#include <QPalette>

namespace Ui {
class CustomSliderWidget;
}

/*!
 * \brief Класс слайдера с полем для выбора значения импульса в мкс
 *
 * Класс слайдера с полем для выбора значения импульса в мкс
 * Унаследован от QWidget
 */
class CustomSliderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomSliderWidget(QWidget *parent = nullptr);
    ~CustomSliderWidget();

    /*!
     * \brief retranslateUI переводит надписи
     */
    void retranslateUI();
    /*!
     * \brief setValue
     * \param newValue - новое значение отображающееся на слайдере
     */
    void setValue(int newValue);

signals:
    /*!
     * \brief impulseClicked вызывается, когда пользователь нажимает кнопку "Принять" и незначение со слайдера в мкс
     */
    void impulseClicked(int);
    /*!
     * \brief clicked вызывается, когда пользователь нажимает на виджет слайдера
     *
     * Нужно для последующей проверки нажатия на фон в ImpulseAdder в onSliderBoxClicked()
     */
    void clicked();

private:
    Ui::CustomSliderWidget *ui;
    /*!
     * \brief value - значение слайдера в мкс
     */
    int value;

private slots:
    /*!
     * \brief onSliderChanged
     * Меняет значение на тексте слайдера при изменении
     */
    void onSliderChanged(int);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // CUSTOMSLIDERWIDGET_H
