#ifndef TIMEAXIS_H
#define TIMEAXIS_H

#include <QVector>
#include <QProgressBar>
#include <QWidget>

#include <graphicItems/pointer.h>

#include "config.h"

#define  ALPHA_ACTIVE 130
#define  ALPHA_NOT_ACTIVE 50

namespace Ui {
class TimeAxis;
}

/*!
 * \brief Класс временной оси для интервальности
 */
class TimeAxis : public QWidget
{
    Q_OBJECT

public:
    explicit TimeAxis(QWidget *parent = nullptr);
    ~TimeAxis();

    /*!
     * \brief getStartTime
     * \return начало активного временного интервала
     */
    int getStartTime();

    /*!
     * \brief setStartTime устанавливает начало активного временного интервала
     * \param newTime
     */
    void setStartTime(int newTime);

    /*!
     * \brief getEndTime
     * \return конец активного временного интервала
     */
    int getEndTime();

    /*!
     * \brief setEndTime устанавливает конец активного временного интервала
     * \param newTime
     */
    void setEndTime(int newTime);

    /*!
     * \brief addNewProgressBar добавляет один интервал и делает его активным
     */
    void addNewProgressBar();

    /*!
     * \brief deleteProgressBar удаляет интервал по индексу i
     * \param i
     */
    void deleteProgressBar(qsizetype i);

    /*!
     * \brief selectProgressBar делает под индексом i интервал активныым
     * \param i
     */
    void selectProgressBar(qsizetype i);

    /*!
     * \brief initSimulation инициализирует симуляцию
     */
    void initSimulation();

    /*!
     * \brief moveSimulation сдвигает время симуляции
     *
     * Сдвигает бегунок симуляции на 1 пиксель и активирует и деактивирует мышцы
     */
    void moveSimulation();
    void setIsAdminModeActive(bool newIsAdminModeActive);
signals:
    /*!
     * \brief startChanged вызывается при изменении начала интервала с помощью бегунка
     */
    void startChanged(int);

    /*!
     * \brief endChanged вызывается при изменении конца интервала с помощью бегунка
     */
    void endChanged(int);

    /*!
     * \brief simulationEnded вызывается при окончании симуляции
     */
    void simulationEnded();

    /*!
     * \brief simulationTimeChanged вызывается при изменении времени симуляции на секунду
     */
    void simulationTimeChanged(int);
private:
    Ui::TimeAxis *ui;
    Pointer *startPointer, *endPointer;
    QLabel *startLabel, *endLabel;
    bool startMoving = false, endMoving = false;
    QPoint pointerOffset;
    int stepInPixels;

    QVector<QProgressBar *> progressBars;
    QVector<QLabel *> endPoints, startPoints;
    QProgressBar *activeProgressBar = nullptr;
    QLabel *activeEndPoint = nullptr, *activeStartPoint = nullptr;
    int simulationTime = -1;
    bool isSimulation = false;

    bool isAdminModeActive = false;

    /*!
     * \brief initPointers инициализирует бегунки
     */
    void initPointers();

    /*!
     * \brief getTimeFromPixels
     * \param x - координата
     * \return время в секундах
     *
     * Обратное getPixelsFromTime(int time)
     */
    int getTimeFromPixels(int x);

    /*!
     * \brief getPixelsFromTime
     * \param time - время в секундах
     * \return координату x соответствующую времени time
     *
     * Обратное getTimeFromPixels(int x)
     */
    int getPixelsFromTime(int time);

    /*!
     * \brief setStartGeometry устанавливат положение начальных элементов интервала
     * \param x
     * \param y
     * \param w
     * \param h
     */
    void setStartGeometry(int x, int y, int w, int h);

    /*!
     * \brief setEndGeometry устанавливат положение конечных элементов интервала
     * \param x
     * \param y
     * \param w
     * \param h
     */
    void setEndGeometry(int x, int y, int w, int h);

    /*!
     * \brief activateProgressBar активирует интервал
     * \param progressBar
     */
    void activateProgressBar(QProgressBar *progressBar);

    /*!
     * \brief deactivateProgressBar деактивирует интервал
     * \param progressBar
     */
    void deactivateProgressBar(QProgressBar *progressBar);
    
    /*!
     * \brief hideActivePoints скрывает нижнюю подпись у интервала
     */
    void hideActivePoints();
    
    /*!
     * \brief showActivePoints показывает нижнюю подпись у интервала
     */
    void showActivePoints();
private slots:
    /*!
     * \brief onPointerPressed вызывается при нажатии на бегунок
     * \param offset - сдвиг координат нажатия
     */
    void onPointerPressed(QPoint offset);

    /*!
     * \brief onPointerReleased вызывается при отпускании бегунка
     */
    void onPointerReleased();
protected:
    /*!
     * \brief mouseMoveEvent ловит событие движения мыши
     * \param event
     *
     * Используется при нажатии на один из бегунков для его сдвига по временной оси
     */
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // TIMEAXIS_H
