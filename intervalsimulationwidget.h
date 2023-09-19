#ifndef INTERVALSIMULATIONWIDGET_H
#define INTERVALSIMULATIONWIDGET_H

#include <QWidget>
#include <QTimer>

#include <bodyParts/body.h>

#include <dotsPairs/miosuit.h>

#define ANIMATION_TICK 40 //ms

namespace Ui {
class IntervalSimulationWidget;
}

class IntervalSimulationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IntervalSimulationWidget(bool isUp, QWidget *parent = nullptr);
    ~IntervalSimulationWidget();
private:
    Ui::IntervalSimulationWidget *ui;
signals:
    /*!
     * \brief done вызывается при нажатии на кнопку ОК
     */
    void done();
private slots:
    /*!
     * \brief onOKClicked вызывается при нажатии на "ОК"
     *
     * В результате вызывает done()
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
    Body *body;
    MioSuit *mioSuit;

    bool isGoing = false; ///< Идёт ли симуляция
    bool isUp; ///< Симулируется верхняя часть тела
    QTimer *timer; ///< Таймер симуляции
};

#endif // INTERVALSIMULATIONWIDGET_H
