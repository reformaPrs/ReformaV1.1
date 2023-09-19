#ifndef BODYPART_H
#define BODYPART_H

#include "config.h"
#include "abstractbodypart.h"
#include "graphicItems/textrect.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>

/*!
 * \brief Класс части тела на главном экране
 *
 * Класс части тела на главном экране, отображает состояние двух мышц одной части тела, по одной мышце на зад и перед человека
 */

class BodyPart : public AbstractBodyPart
{
    Q_OBJECT
public:
    /*!
     * \brief BodyPart
     * \param id - уникальный номер части тела, совпадающий с id соответствующей PopBodyPart
     * \param img1 - массив с путями к ресурсам для картинок первой мышцы
     * \param img2 - массив с путями к ресурсам для картинок второй мышцы
     * \param hint1 - текст подсказки к первой мышце на двух языках
     * \param hint2 - текст подсказки к второй мышце на двух языках
     * \param hintX1
     * \param hintY1
     * \param hintX2
     * \param hintY2
     * \param scene - сцена в которую нужно добавить часть (в других классах реализована функция addToScene)
     */
    explicit BodyPart(int id, QString img1[5], QString img2[5]);

    ~BodyPart();

    /*!
     * \brief The Image enum - перечисление индексов картинок словестно для упрощения работы с ними
     */
    enum Image{
        Highlight,
        Red,
        Green,
        Interval,
        Yellow,
        Images
    };
    Q_ENUM(Image);

    /*!
     * \brief translate
     * \param language
     *
     * Переводит текст подсказок
     */
    void translate(Language language);

    /*!
     * \brief getIntaractive
     * \return true - если часть тела реагирует на нажатие и наведение, false - если нет
     */
    bool getIntaractive() const;
    /*!
     * \brief setIntaractive
     * \param newIntaractive
     *
     * Включает и отключает реакцию части тела на нажания и наведение
     */
    void setIntaractive(bool newIntaractive);

    /*!
     * \brief Скрывает часть тела на сцене
     */
    void hide();
    /*!
     * \brief Показывает часть тела
     */
    void show();

    void addHints(QString hint1[2], QString hint2[2], int hintX1, int hintY1, int hintX2, int hintY2);

signals:
    /*!
     * \brief clicked вызывается при нажатии на часть тела
     * \param id - уникальный номер части тела
     */
    void clicked(int id);

private:
    /*!
     * \brief initMuscles инициализирует свойства firstMuscleи и secondMuscle
     */
    void initMuscles();

    TextRect *hintTextRect1; ///< Объект подсказки первой мышцы
    TextRect *hintTextRect2; ///< Объект подсказки второй мышцы
    QString hintStr1[3]; ///< Текст первой подсказки на двух языках
    QString hintStr2[3]; ///< Текст второй подсказки на двух языках
    /*!
     * \brief intaractive
     *
     * true - реакцию части тела на нажания и наведение включена, false - выключена
     */
    bool intaractive = true;

private slots:
    /*!
     * \brief onMuscleClick вызывается при нажатии на картинку мышцы
     */
    void onMuscleClick();
    /*!
     * \brief onHoverEntered вызывается при наведении на мышцу
     *
     * В результате подсвечивает часть тела и выводит подсказку мышцы
     */
    void onHoverEntered();

public slots:
    /*!
     * \brief onHoverLeft вызывается при выходе из наведённого состояния мышцы
     *
     * В результате убирает подсветку части тела и скрывает подсказку мышцы
     */
    void onHoverLeft();

    // SimpleBodyPart interface
public:

    void setStatus(Status newStatus);
    void addToScene(QGraphicsScene *scene);
};

#endif // BODYPART_H
