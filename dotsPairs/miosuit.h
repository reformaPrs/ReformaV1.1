#ifndef MOISUIT_H
#define MOISUIT_H

#include "dotsPairs/programslidersvalues.h"
#include "dotspair.h"
#include "intervaldotspair.h"
#include "popdotspair.h"


#include <QObject>
#include <QFile>
#include <QException>
#include <QDebug>

#include <bodyParts/heater.h>

/*!
 * \brief Класс костюма
 * Является фасадом для всех пар контактов и нагревателей
 * Также может существовать только один объект данного класса (Singleton)
 */
class MioSuit : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief getMioSuitInstance создаёт единственный объект класса
     * \return объект класса MioSuit
     */
    static MioSuit *getMioSuitInstance();
    ~MioSuit();
    /*!
     * \brief addToScene добавляет пары котнактов и нагреватели на сцену
     * \param scene - сцена для добавления
     */
    void addToScene(QGraphicsScene *scene);
    /*!
     * \brief translate переводит подсказки паар контактов и нагревателей
     * \param language - язык для перевода
     */
    void translate(Language language);
    /*!
     * \brief show отображает активные пары контактов
     */
    void show();
    /*!
     * \brief hide скрывает пары контактов
     */
    void hide();

    /*!
     * \brief hideHeaters скрывает все нагреватели
     */
    void hideHeaters();
    /*!
     * \brief showHeaters показывает все нагреватели
     */
    void showHeaters();

    /*!
     * \brief resetDotsPairs деактивирует пары контактов части тела
     * \param partID - id части тела
     */
    void resetDotsPairs(int partID);

    /*!
     * \brief addPopToScene добавляет всплывающие  контакты на сцену
     * \param partID - id части тела
     * \param scene - сцена для добавления
     */
    void addPopToScene(int partID, QGraphicsScene *scene);
    /*!
     * \brief resetPopDotsPairs деактивирует всплывающие пары контактов
     */
    void resetPopDotsPairs();

    /*!
     * \brief activateMusclePopDotsPairs показвывает всплывающую пару контактов мышцы
     * \param isFirstMuscle - первая из пары мыщц активирована
     */
    void activateMusclePopDotsPairs(bool isFirstMuscle);

    /*!
     * \brief deactivateMusclePopDotsPairs скрывает всплывающую пару контактов мышцы
     * \param isFirstMuscle - первая из пары мыщц деактивирована
     */
    void deactivateMusclePopDotsPairs(bool isFirstMuscle);
    /*!
     * \brief showPopDotsPairs показывает всплывающе контакты
     */
    void showPopDotsPairs();
    /*!
     * \brief removePopFromScene убирает контакты со сцены всплывающего окна
     * \param scene - сцена
     */
    void removePopFromScene(QGraphicsScene *scene);
    /*!
     * \brief isMuscleActive активна ли пара контактов (мышца)
     * \param isFirstMuscle - первая из пары мыщц на проверку
     * \return true - если активна
     */
    bool isMuscleActive(bool isFirstMuscle);
    /*!
     * \brief isPopValuesChanged проверяет поменялись ли значения всплывающих пар контактов
     * \return true - если поменялись
     */
    bool isPopValuesChanged();
    /*!
     * \brief mergeChanges записывает значения всплывающих пар контактов в обычные
     */
    void mergeChanges();
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
     * \brief copyHalfhour копирует информацию на другой получас
     */
    void copyHalfhour();

    /*!
     * \brief addIntervalPairs добавляет новые пары контактов в интервальные пары из активных всплывающих пар
     * \param isUp - относиться ли пара к верхней части тела
     * \return количество добавленных пар
     */
    int addIntervalPairs(bool isUp);
    /*!
     * \brief getIntervalDotsPairAt возвращает интервальную пару контактов по индексу
     * \param index - интекс интервальной пары
     * \param isUp - относиться ли пара к верхней части тела
     * \return интервальная пара контактов
     */
    IntervalDotsPair *getIntervalDotsPairAt(int index, bool isUp);
    /*!
     * \brief removeIntervalDotsPairAt удаляет интервальную пару контактов по индексу
     * \param index - интекс интервальной пары
     * \param isUp - относиться ли пара к верхней части тела
     */
    void removeIntervalDotsPairAt(int index, bool isUp);

    const QVector<IntervalDotsPair *> &getIntervalDotsPairs(bool isUp) const;
    /*!
     * \brief isIntervalDotsPairWithPartID
     * \param partID
     * \param isUp - относиться ли пара к верхней части тела
     * \return
     */
    bool isIntervalDotsPairWithPartID(int partID, bool isUp);
    /*!
     * \brief setDotsPairSimulationValue
     * \param id
     * \param value
     */
    void setDotsPairSimulationValue(int id, int value);

    /*!
     * \brief isFirstMuscleByID относиться ли пара контактов к первой мышце
     * \param id - id пары контактов
     * \return екгу - если относиться к первой мышце
     *
     * Нужно для проверки IntervalDotsPair
     */
    bool isFirstMuscleByID(int id);
    /*!
     * \brief setRecievedValues
     * \param recievedValues
     */
    void setRecievedValues(int recievedValues[40][40]);
    /*!
     * \brief update обновляет пары контактов в соответствии с сохранёнными значениями
    */
    void update();

    /*!
     * \brief saveToFile сохраняет информацию о костюме в файл
     * \param out - поток вывода в файл
     */
    void saveToFile(QDataStream *out);
    /*!
     * \brief readFromFile считывает информацию о костюме из файла
     * \param in - поток ввода из файла
     */
    void readFromFile(QDataStream *in);

    void readFromFile(QByteArray &data);

    /*!
     * \brief activateHalfhour активирует режим получасов
     */
    void activateHalfhour();

    const QVector<DotsPair *> &getDotsPairs() const;
    bool getIsHalfhourActive() const;

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

    void setProgramSlidersValues(const ProgramSlidersValues values);
    ProgramSlidersValues getMinSliderValues();
    ProgramSlidersValues getMaxSliderValues();
    const ProgramSlidersValues &getMaxValues() const;

    void setIsAdminModeActive(bool newIsAdminModeActive);

    void saveInitValuesToFile(QDataStream *out);
    void readInitValuesFromFile(QDataStream *in);

    void clear();
signals:

private:
    class HalfHourException: public QException
    {
    public:
        void raise() const {qDebug() << "\nException: "; throw *this;}
    };

    explicit MioSuit(QObject *parent = nullptr);
    MioSuit(const MioSuit& root) = delete;
    MioSuit& operator=(const MioSuit&) = delete;

    QVector<DotsPair *> dotsPairs; ///< Пары контактов на главной картинке
    QVector<PopDotsPair *> popDotsPairs; ///< Пары контактов для всплывающих окон
    QVector<PopDotsPair *> activePopDotsPairs; ///< Пары контактов для активного всплывающих окон
    QVector<int> pairsValues[2]; ///< Значения импульсов пар контактов для получасов
    QVector<int> initialPairsValues[2]; ///< Изначальные значения импульсов пар контактов для получасов

    bool isHalfhourActive = false; ///< Активен ли режим получаса

    QVector<IntervalDotsPair *> intervalDotsPairsUp[2]; ///< Вектор пар контактов верхнейчасти тела
    QVector<IntervalDotsPair *> intervalDotsPairsDown[2]; ///< Вектор пар контактов нижней части тела
    int halfhourIndex = 0; ///< Индекс активного получаса
    QVector<QString> pairsNames; ///< Имена пар контактов для интервалов

    QVector<Heater *> heaters; ///< Вектор нагревателей

    ProgramSlidersValues programValues;
    ProgramSlidersValues maxValues;

    bool isAdminModeActive = false;
    /*!
     * \brief readDotsPairsFromCSV считывает данные о парах контактов из csv файла
     */
    void readDotsPairsFromCSV();
    /*!
     * \brief readHeatersFromCSV считывает данные о нагревателях из csv файла
     */
    void readHeatersFromCSV();
    /*!
     * \brief checkForActiveHalfhour проверяет записанные данные получасов
     *
     * Если данные получасов равны, то получасы деактивируются
     */
    void checkForActiveHalfhour();
    void setMaxValue(int pairIndex, int halfhourIndex, int value);
    void showInitialValues();
    void showSlidersProgramedValues();
};

#endif // MOISUIT_H
