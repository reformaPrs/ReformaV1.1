#ifndef SIMPLEBODYPART_H
#define SIMPLEBODYPART_H

#include "hoverpixmapitem.h"

#include <QGraphicsScene>
#include <QObject>

/*!
 * \brief The AbstractBodyPart class - абстрактный класс части тела
 */
class AbstractBodyPart : public QObject
{
    Q_OBJECT
public:
    explicit AbstractBodyPart(int id, QObject *parent = nullptr);
    ~AbstractBodyPart();

    /*!
     * \brief The Status enum - статус мышцы
     *
     * Правильнее было бы ввести его в классе мышцы и унаследовать от него классы BodyPart и PopBodyPart, можно исправить в будующем
     */
    enum Status{
        BothDisabled, ///< Выключены обе мышцы
        FirstEnabled, ///< Включена только первая мышца
        SecondEnabled, ///< Включена только вторая мышца
        BothEnabled, ///< Включены обе мышцы
        Intervals ///< Мышцы учавствуют в интервальности
    };
    Q_ENUM(Status)

    /*!
     * \brief getId
     * \return id - уникальный номер части тела
     */
    int getId() const;

    /*!
     * \brief setStatus
     * \param newStatus
     *
     * Изменяет snatus части тела на newStatus, при этом меняет отображение картинок мышц в соответствии с новым статусом
     */
    virtual void setStatus(Status newStatus) = 0;
    /*!
     * \brief getStatus
     * \return status части тела
     */
    Status getStatus() const;

    /*!
     * \brief addToScene добавляет часть тела на сцену
     * \param scene
     */
    virtual void addToScene(QGraphicsScene *scene) = 0;
signals:

private:
    int id;
protected:
    /*!
     * \brief firstMuscle - картинки для первой мышцы разного цвета
     */
    QVector<HoverPixmapItem *> firstMuscle;
    /*!
     * \brief secondMuscle - картинки для второй мышцы разного цвета
     */
    QVector<HoverPixmapItem *> secondMuscle;
    /*!
     * \brief status - статус части тела
     */
    Status status;
};

#endif // SIMPLEBODYPART_H
