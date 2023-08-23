#include "customprogressbar.h"

#include <QTimer>
#include <QThread>

CustomProgressBar::CustomProgressBar(QWidget *parent) : QProgressBar(parent)
{
}

void CustomProgressBar::updateProgressInThread(int targetValue)
{
    int startValue = value();
    int endValue = targetValue;
    int elapsedMs = 0;
    int durationMs = 700; // Значение по умолчанию, можно передать другое

    while (elapsedMs < durationMs) {
        elapsedMs += 10; // Имитация прошедшего времени
        int newValue = startValue + (endValue - startValue) * elapsedMs / durationMs;

        setValue(newValue);
        QThread::msleep(10);
    }

    QMetaObject::invokeMethod(this, "setValue", Qt::QueuedConnection, Q_ARG(int, endValue));
}

void CustomProgressBar::paintEvent(QPaintEvent *event)
{
    QProgressBar::paintEvent(event);
}
