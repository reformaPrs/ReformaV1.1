#ifndef HEATER_H
#define HEATER_H

#include "hoverpixmapitem.h"
#include "graphicItems/dot.h"
#include "graphicItems/textrect.h"
#include "config.h"

#include <QObject>
#include <QGraphicsScene>

using namespace std;

/*!
 * \brief Класс нагревательного элемента
 */
class Heater : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Heater
     * \param id - уникальный номер нагревателя
     * \param imgs - пути к ресурсам картинок
     * \param parent
     */
    explicit Heater(int id, QVector<QString> imgs, QObject *parent = nullptr);
    ~Heater();

    /*!
     * \brief Скрывает нагреватель на сцене
     */
    void hide();
    /*!
     * \brief Показывает нагреватель
     */
    void show();

    /*!
     * \brief getIsActive
     * \return true - если нагреватель включён и false - если нет
     */
    bool getIsActive() const;
    /*!
     * \brief setIsActive
     * \param newIsActive
     *
     * Включает и выключает нагреватель в зависимости  от newIsAvtive, при этом меняет отображение соответственно режиму
     */
    void setIsActive(bool newIsActive);
    /*!
     * \brief addDot - добавляет новую точку в нагреватель
     * \param x
     * \param y
     * \param r
     *
     * После вызова addDot() обязательно нужно добавить подсказку для данной точки функцией addHint()
     *
     * Возможно стоит объединить функции
     */
    void addDot(int x, int y, int r);
    /*!
     * \brief addHint - добавляет новую подсказку в нагреватель
     * \param x
     * \param y
     * \param hintRU
     * \param hintEN
     *
     * Обязательно вызывается вместе с addDot()
     */
    void addHint(int x, int y, QString hintRU, QString hintEN);
    /*!
     * \brief addToScene - добавляет нагреватель на сцену
     * \param scene
     */
    void addToScene(QGraphicsScene *scene);
    /*!
     * \brief translate - переводит подсказки
     * \param language
     */
    void translate(Language language);

    void setIsAdminModeActive(bool newIsAdminModeActive);

signals:
    /*!
     * \brief clicked вызывается при нажатии на нагреватель
     * \param id - уникальный номер нагревателя
     */
    void clicked(int id);
private:

    QVector<HoverPixmapItem *> heatPoints; ///< Картинки для нагревателя спереди и сзади
    QVector<Dot *> dots; ///< Точки нагревателя

    QVector<TextRect *> hints; ///< Объекты подсказок нагревателя
    QVector<QString> hintsRU; ///< Тексты подсказок на русском
    QVector<QString> hintsEN; ///< Тексты подсказок на английском

    int id; ///< Уникальный номер нагревателя
    /*!
     * \brief Активен ли нагреватель
     *
     * true - включён
     * false - выключен
     */
    bool isActive;

    bool isAdminModeActive = false;
private slots:
    /*!
     * \brief onHeaterClick вызывается при нажатии на нагреватель
     *
     * В результате меняет отображение на другой режим и высылает ckicked()
     */
    void onHeaterClick();
    /*!
     * \brief onDotEntered вызывается при наведение на точку
     *
     * В результате показвыет подсказку
     */
    void onDotEntered();
    /*!
     * \brief onDotLeft вызывается при выходе из наведения на точку
     *
     * В результате скрывает подсказку
     */
    void onDotLeft();
};

#endif // HEATER_H
