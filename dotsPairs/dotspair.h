#ifndef DOTSPAIR_H
#define DOTSPAIR_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>


#include "graphicItems/dot.h"
#include "config.h"
#include "dotsPairs/simpledotspair.h"

/*!
 * \brief Класс пары контактов на главном экране
 */
class DotsPair : public SimpleDotsPair
{
    Q_OBJECT
public:
    DotsPair(int x1, int y1, int x2, int y2, int radius, int partID, int id, bool firstMuscle);
    virtual ~DotsPair();

    /*!
     * \brief addToScene добавляет пару контактов на сцену
     * \param scene
     */
    virtual void addToScene(QGraphicsScene *scene);

    /*!
     * \brief removeFromScene убирает со сцены
     * \param scene
     */
    virtual void removeFromScene(QGraphicsScene *scene);

    /*!
     * \brief hide скрывает со сцены
     */
    virtual void hide();
    /*!
     * \brief show показывает на сцене
     */
    void show();

    virtual void setValue(int newValue) override;

    /*!
     * \brief getIsFirstMuscle
     * \return
     */
    bool getIsFirstMuscle() const;

protected:
    /*!
     * \brief setAcceptHoverEvents устанавливает отслеживание событий с наведением для обоих точек
     * \param hover
     */
    void setAcceptHoverEvents(bool hover);
    /*!
     * \brief getRadius
     * \return radius - радиус точек
     */
    int getRadius() const;
    /*!
     * \brief getDot1
     * \return dot1 - объект первой точки
     */
    Dot *getDot1() const;
    /*!
     * \brief getDot2
     * \return dot2 - объект второй точки
     */
    Dot *getDot2() const;

    void setOffset(QPoint offset);

private:
    QGraphicsRectItem *rect; ///< Прямоугольник (рамка для текста)
    Dot *dot1; ///< Объект первой точки (контакта)
    Dot *dot2; ///< Объект второй точки (контакта)
    QGraphicsLineItem *line; ///< Линия соединяющая точки
    QGraphicsTextItem *text; ///< Текст с значением импульса в мкс

    int x1; ///< x первой точки
    int x2; ///< x второй точки
    int y1; ///< y первой точки
    int y2; ///< y второй точки
    int radius; ///< радиус точек
    QPoint textOffset;

    bool isFirstMuscle; ///< Относиться ли пара контактов к первой мышце

};

#endif // DOTSPAIR_H
