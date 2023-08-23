#ifndef BODY_H
#define BODY_H

#include "bodypart.h"
#include "popbodypart.h"

#include <QObject>
#include <QFile>
#include <QException>

#include <dotsPairs/dotspair.h>
#include <dotsPairs/intervaldotspair.h>

/*!
 * \brief The Body class - класс фасад всего тела.
 *
 * Управляет всеми частями тела, может существовать только один экземпляр (Singleton)
 */
class Body : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief getBodyInstance создаёт единственный объект класса Body
     * \return объект класса Body
     */
    static Body *getBodyInstance();
    ~Body();

    /*!
     * \brief The ActiveBodyPart enum - варианты состояний интерактивности тела
     *
     * Вехняя часть тела
     * Нижняя часть тела
     * Всё тело
     */
    enum ActiveBodyPart{
        UP_BODY, ///< Вехняя часть тела (рубашка)
        DOWN_BODY, ///< Нижняя часть тела (штаны)
        WHOLE_BODY, ///< Всё тело
        NONE ///< Ничего
    };
    Q_ENUM(ActiveBodyPart)
    /*!
     * \brief addToScene добавляет на сцену изображение человека и все части тела
     * \param scene - сцена для добавления
     */
    void addToScene(QGraphicsScene *scene);

    /*!
     * \brief addPopToScene добавляет выбранную часть тела в увеличенном варианте на всплывающее окно
     * \param scene - сцена для добавления
     * \param id - id части тела
     * \param dialog - всплывающее окно для подключения слотов (не очень удачно вышло, что имена слотов определяются здесь)
     */
    void addPopToScene(QGraphicsScene *scene, int id, const QObject *dialog);
    /*!
     * \brief removePopFromScene - убирает часть тела в увеличенном варианте со сцены
     * \param scene - сцена с чатью тела
     * \param id - id части тела
     */
    void removePopFromScene(QGraphicsScene *scene, int id);

    /*!
     * \brief translate переводит тексты подсказок частей тела на общей картинке
     * \param language - локаль, на которую нужно перевести
     */
    void translate(Language language);

    /*!
     * \brief getPopStatus возвращает статус всплывающей части тела
     * \param id - id части тела
     * \return Статус всплывающей части тела
     */
    AbstractBodyPart::Status getPopStatus(int id) const;
    /*!
     * \brief setPopStatus устанавливает новый статус всплывающей части тела
     * \param id - id части тела
     * \param status - новый статус
     */
    void setPopStatus(int id, AbstractBodyPart::Status status);

    /*!
     * \brief setPartStatus устанавливает новый статус части тела
     * \param id - id части тела
     * \param status - новый статус
     */
    void setPartStatus(int id, AbstractBodyPart::Status status);

    /*!
     * \brief hide скрывает все части тела, но не фон (всё тело)
     */
    void hide();

    /*!
     * \brief show показывает все активные части тела
     */
    void show();
    /*!
     * \brief mergeChanges сохраняет статутс всплывающей части тела в статус обычной
     * \param id - id частей тела
     */
    void mergeChanges(int id);
    /*!
     * \brief cancelChanges сохраняет статутс обычной части тела в статус всплывающей
     * \param id - id частей тела
     */
    void cancelChanges(int id);
    /*!
     * \brief setIntaractive обновляет интерактивность частей тела в зависимости от значения activePart
     * \param activePart
     */
    void setIntaractive(ActiveBodyPart activePart);

    /*!
     * \brief setHalfhourIndex задаёт новый индекс получаса
     * \param newHalfhourIndex - новый индекс
     *
     * 0 - первый получас
     * 1 - второй получас
     */
    void setHalfhourIndex(int newHalfhourIndex);
    /*!
     * \brief saveHalfhour сохраняет данные о текущем получасе
     */
    void saveHalfhour();
    /*!
     * \brief resetIntervalParts делает интервальные части тела не активными
     *
     * Нужно для симуляции
     */
    void resetIntervalParts();
    /*!
     * \brief copyHalfhour копирует информацию на другой получас
     */
    void copyHalfhour();
    /*!
     * \brief setCalculatePartStatus устанавливает статус части тела на активный в зависимости от поданного значения isFirst
     * \param partID - id части тела
     * \param isFirst - первая ли мышца активна
     *
     * Если одна из мышц уже активна, а подаётся что активна и вторая, то активны станут обе, а не только поданная
     */
    void setCalculatePartStatus(int partID, bool isFirst);

    /*!
     * \brief clear деактивирует все части тела
     */
    void clear();
    /*!
     * \brief update обновляет мышцы в соответсвии с сохранённой информацией
     */
    void update();
    /*!
     * \brief update обновляет мышцы в соответствии с значениями полученных пар
     * \param dotsPairs - пары контактов
     */
    void update(const QVector<DotsPair *> dotsPairs);
    /*!
     * \brief update обновляет мышцы в соответствии с значениями полученных пар
     * \param dotsPairs - пары контактов
     */
    void update(const QVector<IntervalDotsPair *> IntervalDotsPairs);

    /*!
     * \brief saveToFile сохраняет статусы мышц в файл
     * \param out - поток вывода в файл
     */
    void saveToFile(QDataStream *out);
    /*!
     * \brief readFromFile считывает статусы мышц из файла
     * \param in - поток ввода из файла
     */
    void readFromFile(QDataStream *in);

    /*!
     * \brief activateHalfhour активирует режим получасов
     */
    void activateHalfhour();
    /*!
     * \brief clearHalfhour
     *
     * Очищает получас при Ctrl+N
     */
    void clearHalfhour();
    /*!
     * \brief deactivateHalfhour отключает режим получасов
     */
    void deactivateHalfhour();

    void setIsAdminModeActive(bool newIsAdminModeActive);

signals:
    /*!
     * \brief clickedBodyPart вызывается при нажатии на часть тела
     */
    void clickedBodyPart(int);

    /*!
     * \brief clickedBackground вызывается при нажатии на фон
     */
    void clickedBackground();

private slots:
    /*!
     * \brief onBodyPartClicked вызывается при нажатии на часть тела
     * \param id - id части тела
     *
     * Вызывает clickedBodyPart(int)
     */
    void onBodyPartClicked(int id);

    /*!
     * \brief onBackgroundClicked вызывается при нажатии на фон
     *
     * Вызывает clickedBackground()
     */
    void onBackgroundClicked();

private:
    class HalfHourException: public QException
    {
    public:
        void raise() const {qDebug() << "\nException: "; throw *this;}
    };

    explicit Body(QObject *parent = nullptr);
    Body(const Body& root) = delete;
    Body& operator=(const Body&) = delete;
    HoverPixmapItem *body; ///< Картинка с телом
    BodyPart *bodyParts[18]; ///< Части тела на главной картинке
    AbstractBodyPart::Status bodyPartsStatuses[2][18]; ///< Статусы частей тела для получасов
    int halfhourIndex = 0; ///< Индекс активного получаса
    PopBodyPart *popBodyParts[18]; ///< Части тела на всплывающем окне

    bool isHalfhourActive = false; ///< Активен ли режим получаса
    bool isIntervalActive = false; ///< Активно ли добаление в интервалы
    QVector<BodyPart *> activeIntervalBodyParts; ///< Вектор активных частей тела для отображения во аремя симуляции
    QVector<QString> pairsNames; ///< Имена пар контактов для интервалов

    AbstractBodyPart::Status oldPopStatus; ///< Статус активной всплывающей части тела до редактирования

    bool isAdminModeActive = false;
    /*!
     * \brief readBodyPartsFromCSV считывает данные о частях тела для главного окна из csv файла
     */
    void readBodyPartsFromCSV();
    /*!
     * \brief readPopBodyPartsFromCSV считывает данные о частях тела для всплывающих окон из csv файла
     */
    void readPopBodyPartsFromCSV();
};

#endif // BODY_H
