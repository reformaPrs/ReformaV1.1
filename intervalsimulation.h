#ifndef INTERVALSIMULATION_H
#define INTERVALSIMULATION_H

#include <QDialog>
#include <QPixmap>
#include <QTimer>

#include <bodyParts/body.h>

#include <dotsPairs/miosuit.h>

#define ANIMATION_TICK 40 //ms

namespace Ui {
class IntervalSimulation;
}

/*!
 * \brief Класс диалогового окна симуляции интервальности
 *
 * Не используется в данной версии
 */
class IntervalSimulation : public QDialog
{
    Q_OBJECT

public:
    explicit IntervalSimulation(bool isUp, QWidget *parent = nullptr);
    ~IntervalSimulation();

signals:

private slots:
    /*!
     * \brief onOKClicked вызывается при нажатии на "ОК"
     *
     * В результате закрывает окно
     */
    void onOKClicked();

    /*!
     * \brief onPlayClicked вызывается при нажатии на кнопку пуска
     */
    void onPlayClicked();

    /*!
     * \brief onTimerTimeout вызывается при тике таймера
     */
    void onTimerTimeout();

    /*!
     * \brief onSimulationEnd вызывается при окончании симуляции
     */
    void onSimulationEnd();

    /*!
     * \brief onSimulationTimeChanged вызывается каждую секунду симуляции
     * \param seconds
     */
    void onSimulationTimeChanged(int seconds);
private:
    Ui::IntervalSimulation *ui;

    Body *body;
    MioSuit *mioSuit;

    bool isGoing = false; ///< Идёт ли симуляция
    bool isUp; ///< Симулируется верхняя часть тела
    QTimer *timer; ///< Таймер симуляции
};

#endif // INTERVALSIMULATION_H
