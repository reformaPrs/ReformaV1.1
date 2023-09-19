#ifndef CUSTOMPROGRESSBAR_H
#define CUSTOMPROGRESSBAR_H

#include <QObject>
#include <QProgressBar>

class CustomProgressBar : public QProgressBar
{
public:
    CustomProgressBar(QWidget* parent = nullptr);
public slots:
    void updateProgressInThread(int targetValue);
private:
    void paintEvent(QPaintEvent* event) override;
};


#endif // CUSTOMPROGRESSBAR_H
