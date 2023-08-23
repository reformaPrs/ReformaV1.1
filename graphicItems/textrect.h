#ifndef TEXTRECT_H
#define TEXTRECT_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

/*!
 * \brief Класс текста с рамкой
 *
 * Текск оказывается в прямоугольной рамке.
 * Если длина текста не влезает в ширину прямоугольника, то происходит перенос на новую строку.
 */
class TextRect : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief The Alignment enum - Два варианта задачи координат
     *
     * Center - координатами задаётся центр прямоугольника
     * Corner - координатами задаётся верхний левый угол прямоугольника
     */
    enum Alignment{
        Center,
        Corner
    };
    Q_ENUM(Alignment)

    /*!
     * \brief TextRect
     * \param str - текст
     * \param x - координата
     * \param y - координата
     * \param alignment - вариант задачи координат
     */
    explicit TextRect(QString str, int x, int y, Alignment alignment = Center);
    ~TextRect();

    /*!
     * \brief addToScene добавляет на сцену
     * \param scene
     */
    void addToScene(QGraphicsScene *scene);
    /*!
     * \brief removeFromScene убирает со сцены
     * \param scene
     */
    void removeFromScene(QGraphicsScene *scene);
    /*!
     * \brief hide скрывает объект на сцене
     */
    void hide();
    /*!
     * \brief show показывает объект на сцене
     */
    void show();
    /*!
     * \brief setText устанавливает новый текст
     * \param newText
     *
     * Высота прямоугольника при этом пересчитывается
     */
    void setText(QString newText);
    /*!
     * \brief setWidth устанавливает новую ширину рамки
     * \param newWidth
     *
     * При этом текст заново переноситься и вычисляется новая высота рамки
     */
    void setWidth(int newWidth);
    /*!
     * \brief setFont устанавливает новый шрифт текста
     * \param font
     */
    void setFont(QFont font);

private:
    QGraphicsTextItem *text; ///< Объект текста
    QGraphicsRectItem *rect; ///< Объект прямоугольника
    Alignment alignment; ///< Выравнивание текста относительно заданных координат
    int x; ///< x текста
    int y; ///< y текста
    int width; ///< ширина текста
};

#endif // TEXTRECT_H
