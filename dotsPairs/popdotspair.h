#ifndef POPDOTSPAIR_H
#define POPDOTSPAIR_H

#include "dotspair.h"
#include "customsliderwidget.h"
#include "graphicItems/textrect.h"
/*!
 * \brief Класс пары контактов для всплывающих окон
 */
class PopDotsPair : public DotsPair
{
    Q_OBJECT
public:
    PopDotsPair(int x1, int y1, int x2, int y2, int radius, int partID, int id, bool firstMuscle, QString hintStr1[2], QString hintStr2[2], int hintX[2], int hintY[2]);
    PopDotsPair(int x1, int y1, int x2, int y2, int radius, int partID, int id, bool firstMuscle, QString hint1[2], QString hint2[2], int hintX[2], int hintY[2], int sliderX, int sliderY);
    ~PopDotsPair();

    /*!
     * \brief translate переводит подсказки точек
     * \param language
     */
    void translate(Language language);

private:

    TextRect *hint1; ///< Объект первой подсказки
    TextRect *hint2; ///< Объект второй подсказки
    QString hintStr1[2]; ///< Текст первой подсказки на двух языках
    QString hintStr2[2]; ///< Текст второй подсказки на двух языках
    CustomSliderWidget *slider; ///< Объект виджета слайдера

private slots:
    /*!
     * \brief onImpulseClicked вызывается при сигнале от слайдера о принятии значения
     *
     * В результате меняет значение импульса и отображение точек
     */
    void onImpulseClicked(int);
    /*!
     * \brief onDotHoverEntered вызывается при наведении на точки
     */
    void onDotHoverEntered();
    /*!
     * \brief onDotHoverLeft вызывается при выходе из наведения на точки
     */
    void onDotHoverLeft();
    /*!
     * \brief onDotClicked вызывается при нажатии на точку
     *
     * В результате выводит слайдер для изменения значения импульса
     */
    void onDotClicked();

    // DotsPair interface
public:
    void addToScene(QGraphicsScene *scene);
    void removeFromScene(QGraphicsScene *scene);
    void hide();
    void setValue(int newValue);
};

#endif // POPDOTSPAIR_H
