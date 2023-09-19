#ifndef POPBODYPART_H
#define POPBODYPART_H
#define SCALED_WIDTH 450

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "hoverpixmapitem.h"
#include "abstractbodypart.h"
#include "config.h"

using namespace std;
/*!
 * \brief Класс части тела для всплывающего окна
 */
class PopBodyPart : public AbstractBodyPart
{
    Q_OBJECT
public:
    /*!
     * \brief PopBodyPart
     * \param id - уникальный номер части тела совпадающий с id соответствующей BodyPart
     * \param img1 - массив с путями к ресурсам для картинок первой мышцы
     * \param img2 - массив с путями к ресурсам для картинок второй мышцы
     *
     * В отличие от BodyPart вместо одной картинки с синей мышцой в каждом массиве есть картинка заднего фона
     */
    explicit PopBodyPart(int id, QString img1[5], QString img2[5]);

    ~PopBodyPart();

    /*!
     * \brief The Image enum - перечисление индексов картинок словестно для упрощения работы с ними
     */
    enum Image{
        Highlight,
        Red,
        Green,
        Yellow,
        Background,
        Images
    };
    Q_ENUM(Image);


    /*!
     * \brief removeFromScene удаляет часть тела со сцены
     * \param scene
     *
     * Необходимо чтобы удалить сцену без удаления объекта
     */
    void removeFromScene(QGraphicsScene *scene);
signals:
    /*!
     * \brief clicked вызывается при нажатии на часть тела
     * \param isMuscleClicked
     */
    void clicked(bool isMuscleClicked);
    /*!
     * \brief backgroundClicked вызывается при нажатии на картинку заднего фона
     */
    void backgroundClicked();

private:
    /*!
     * \brief initMuscles инициализирует свойства firstMuscleи и secondMuscle
     */
    void initMuscles();

    HoverPixmapItem *firstBackground; ///< Картинка первого заднего фона
    HoverPixmapItem *secondBackground; ///< Картинка второго заднего фона

    bool wasMuscleClicked = false; ///< Значение для проверки была ли нажата мышца или задний фон

private slots:
    /*!
     * \brief onMuscleClicked вызывается при нажатии на картинку мышцы
     */
    void onMuscleClicked();
    /*!
     * \brief onHoverEntered вызывается при наведении на мышцу
     *
     * В результате подсвечивает часть тела и выводит подсказку мышцы
     */
    void onHoverEntered();
    /*!
     * \brief onHoverLeft вызывается при выходе из наведённого состояния мышцы
     *
     * В результате убирает подсветку части тела и скрывает подсказку мышцы
     */
    void onHoverLeft();
    /*!
     * \brief onBackgroundClicked вызывается при нажатии на картинку заднего фона
     */
    void onBackgroundClicked();

    // SimpleBodyPart interface
public:
    void setStatus(Status newStatus);
    void addToScene(QGraphicsScene *scene);
};

#endif // POPBODYPART_H
