#include "timeaxis.h"
#include "ui_timeaxis.h"

TimeAxis::TimeAxis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeAxis)
{
    ui->setupUi(this);
    setFixedSize(640, 70);

    stepInPixels = ui->backgroundProgressBar->width()/MAX_INTERVAL;

    QLabel *zeroLabel = new QLabel("0", this);
    QLabel *maxLabel = new QLabel(QString::number(MAX_INTERVAL), this);

    int x = ui->backgroundProgressBar->x();
    int y = ui->backgroundProgressBar->y() + ui->backgroundProgressBar->height();
    zeroLabel->setGeometry(x, y - zeroLabel->height() / 4, zeroLabel->width(), zeroLabel->height());

    x += ui->backgroundProgressBar->width();
    maxLabel->setGeometry(x, y - maxLabel->height() / 4, maxLabel->width(), maxLabel->height());

    initPointers();
}

TimeAxis::~TimeAxis()
{
    delete startLabel;
    delete endLabel;
    delete startPointer;
    delete endPointer;
    for (auto progressBar : progressBars){
        delete progressBar;
    }
    for (auto point : endPoints){
        delete point;
    }
    for (auto point : startPoints){
        delete point;
    }
    delete ui;
}

int TimeAxis::getStartTime()
{
    return getTimeFromPixels(startPointer->x() + startPointer->width());
}

void TimeAxis::setStartTime(int newTime)
{
    int w = startPointer->width(), h = startPointer->height(),
            x = getPixelsFromTime(newTime) - w,
            y = ui->backgroundProgressBar->y() - h;

    setStartGeometry(x, y, w, h);
}

int TimeAxis::getEndTime()
{
    return getTimeFromPixels(endPointer->x());
}

void TimeAxis::setEndTime(int newTime)
{
    int w = endPointer->width(), h = endPointer->height();
    int x = getPixelsFromTime(newTime);
    int y =  ui->backgroundProgressBar->y() - h;

    setEndGeometry(x, y, w, h);
}

void TimeAxis::hideActivePoints()
{
    activeStartPoint->hide();
    activeEndPoint->hide();
}

void TimeAxis::activateProgressBar(QProgressBar *progressBar)
{
    progressBar->setValue(100);
    progressBar->setStyleSheet("QProgressBar{"
                                     "color:rgba(0,0,0,0);"
                                     "border-radius: 5px;"
                                     "background-color: rgba(0,255,0,0)"
                                         "}"
                                     "QProgressBar::chunk{"
                                     "background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0.8 rgba(4, 199, 33, " + QString::number(ALPHA_ACTIVE) +
                               "),stop:0 rgba(66, 160, 0, " + QString::number(ALPHA_ACTIVE) +"))"
                                     "}");
}

void TimeAxis::showActivePoints()
{
    int x = activeProgressBar->x();
    int y = ui->backgroundProgressBar->y() + ui->backgroundProgressBar->height();
    activeStartPoint->setText(startLabel->text());
    activeStartPoint->setGeometry(x, y -  activeStartPoint->height() / 4,
                                  activeStartPoint->width(), activeStartPoint->height());
    activeStartPoint->show();

    x = activeProgressBar->x() + activeProgressBar->width();
    y = ui->backgroundProgressBar->y() + ui->backgroundProgressBar->height();
    activeEndPoint->setText(endLabel->text());
    activeEndPoint->setGeometry(x, y -  activeEndPoint->height() / 4,
                                  activeEndPoint->width(), activeEndPoint->height());
    activeEndPoint->show();
}

void TimeAxis::deactivateProgressBar(QProgressBar *progressBar)
{
    progressBar->setStyleSheet("QProgressBar{"
                                     "color:rgba(0,0,0,0);"
                                     "border-radius: 5px;"
                                     "background-color: rgba(0,255,0,0)"
                                         "}"
                                     "QProgressBar::chunk{"
                                     "background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0.8 rgba(4, 199, 33, " + QString::number(ALPHA_NOT_ACTIVE) +
                               "),stop:0 rgba(66, 160, 0, " + QString::number(ALPHA_NOT_ACTIVE) +"))"
                                     "}");
}

void TimeAxis::addNewProgressBar()
{
    if (activeProgressBar != nullptr) {
        deactivateProgressBar(activeProgressBar);
        showActivePoints();
    }
    activeProgressBar = new QProgressBar(this);
    progressBars.append(activeProgressBar);
    activeStartPoint = new QLabel(this);
    activeEndPoint = new QLabel(this);
    startPoints.append(activeStartPoint);
    endPoints.append(activeEndPoint);

    activateProgressBar(activeProgressBar);
    hideActivePoints();
    activeProgressBar->show();
    setStartTime(0);
    setEndTime(MAX_INTERVAL);
}

void TimeAxis::deleteProgressBar(qsizetype i)
{
    if (progressBars[i] != nullptr) {
        delete progressBars[i];
        delete startPoints[i];
        delete endPoints[i];
        progressBars.removeAt(i);
        startPoints.removeAt(i);
        endPoints.removeAt(i);
        activeProgressBar = nullptr;
    }
    startPointer->hide();
    endPointer->hide();
    startLabel->hide();
    endLabel->hide();
}

void TimeAxis::selectProgressBar(qsizetype i)
{
    if (activeProgressBar != nullptr) {
        deactivateProgressBar(activeProgressBar);
        showActivePoints();
    }
    activeProgressBar = progressBars[i];
    activeStartPoint = startPoints[i];
    activeEndPoint = endPoints[i];
    activateProgressBar(activeProgressBar);
    hideActivePoints();

    activeProgressBar->raise();
    int newStartSec = getTimeFromPixels(activeProgressBar->x()),
            newEndSec = getTimeFromPixels(activeProgressBar->x() + activeProgressBar->width());
    setStartTime(newStartSec);
    setEndTime(newEndSec);
}

void TimeAxis::initSimulation()
{
    addNewProgressBar();
    isSimulation = true;
    setEndTime(0);
}

void TimeAxis::moveSimulation()
{
    if (activeProgressBar->width() == ui->backgroundProgressBar->width()) {
        emit simulationEnded();
        setEndTime(0);
    }
    int x = activeProgressBar->x();
    int y = activeProgressBar->y();
    int w = activeProgressBar->width() + 1;
    int h = activeProgressBar->height();
    activeProgressBar->setGeometry(x, y, w, h);
    endPointer->setGeometry(x + w, endPointer->y(), endPointer->width(), endPointer->height());
    endLabel->setGeometry(x + w, endLabel->y(), endLabel->width(), endLabel->height());
    if(simulationTime < getTimeFromPixels(x + w)){
        simulationTime++;
        emit simulationTimeChanged(simulationTime);
        endLabel->setText(QString::number(simulationTime));
        for (auto progressBar : progressBars) {
            if (progressBar->x() <= x + w && progressBar->x()+progressBar->width() > x + w){
                activateProgressBar(progressBar);
            } else if (progressBar->x()+progressBar->width() <= x + w) {
                deactivateProgressBar(progressBar);
            }
        }
    }
}

void TimeAxis::setIsAdminModeActive(bool newIsAdminModeActive)
{
    isAdminModeActive = newIsAdminModeActive;
}

void TimeAxis::initPointers(){


    startLabel = new QLabel("0", this);
    endLabel = new QLabel(QString::number(MAX_INTERVAL), this);

    QPixmap startPointerPic = QPixmap(":/icons/icons/startSlider.png");
    QPixmap endPointerPic = QPixmap(":/icons/icons/endSlider.png");

    startPointer = new Pointer(this);
    endPointer = new Pointer(this);
    startPointer->setPixmap(startPointerPic);
    endPointer->setPixmap(endPointerPic);

    int w = startPointerPic.width(), h = startPointerPic.height(),
            x = ui->backgroundProgressBar->x() - w,
            y =  ui->backgroundProgressBar->y() - h;
    startPointer->setGeometry(x, y, w, h);
    startLabel->setGeometry(x - w / 2, y - startLabel->height(),
                            startLabel->width(), startLabel->height());

    w = endPointerPic.width();
    h = endPointerPic.height();
    x = ui->backgroundProgressBar->x() + ui->backgroundProgressBar->width();
    y =  ui->backgroundProgressBar->y() - h;
    endPointer->setGeometry(x, y, w, h);
    endLabel->setGeometry(x, y - endLabel->height(),
                          endLabel->width(), endLabel->height());

    connect(startPointer, SIGNAL(pressed(QPoint)), this, SLOT(onPointerPressed(QPoint)));
    connect(startPointer, SIGNAL(released()), this, SLOT(onPointerReleased()));
    connect(endPointer, SIGNAL(pressed(QPoint)), this, SLOT(onPointerPressed(QPoint)));
    connect(endPointer, SIGNAL(released()), this, SLOT(onPointerReleased()));

    startPointer->hide();
    endPointer->hide();
    startLabel->hide();
    endLabel->hide();
}


int TimeAxis::getTimeFromPixels(int x)
{
    return (x - ui->backgroundProgressBar->x()) / stepInPixels;
}

int TimeAxis::getPixelsFromTime(int time)
{
    return time * stepInPixels  + ui->backgroundProgressBar->x();
}

void TimeAxis::setStartGeometry(int x, int y, int w, int h)
{
    startLabel->show();
    endLabel->show();
    startPointer->show();
    endPointer->show();
    startLabel->setText(QString::number(getTimeFromPixels(x + w)));
    startPointer->setGeometry(x, y, w, h);
    startLabel->setGeometry(x - w / 2, y - startLabel->height()/ 1.2,
                            startLabel->width(), startLabel->height());
    if (activeProgressBar != nullptr)
        activeProgressBar->setGeometry(x + w, ui->backgroundProgressBar->y(),
                                       endPointer->x() - x - w, ui->backgroundProgressBar->height());
    emit startChanged(getTimeFromPixels(x + w));
}

void TimeAxis::setEndGeometry(int x, int y, int w, int h)
{
    startLabel->show();
    endLabel->show();
    startPointer->show();
    endPointer->show();
    endLabel->setText(QString::number(getTimeFromPixels(x)));
    endPointer->setGeometry(x, y, w, h);
    endLabel->setGeometry(x, y - endLabel->height() / 1.2,
                          endLabel->width(), endLabel->height());
    if (activeProgressBar != nullptr)
        activeProgressBar->setGeometry(activeProgressBar->x(),  ui->backgroundProgressBar->y(),
                                       x - activeProgressBar->x(),  ui->backgroundProgressBar->height());
    emit endChanged(getTimeFromPixels(x));
}

void TimeAxis::onPointerPressed(QPoint offset)
{
    if (!isAdminModeActive || isSimulation) return;
    pointerOffset = offset;
    if (sender() == startPointer)
        startMoving = true;
    else
        endMoving = true;
}

void TimeAxis::onPointerReleased()
{
    if (!isAdminModeActive || isSimulation) return;
    startMoving = false;
    endMoving = false;
}

void TimeAxis::mouseMoveEvent(QMouseEvent *event)
{
    if (startMoving) {
        int w = startPointer->width(), h = startPointer->height();
        int x = event->pos().x() - pointerOffset.x();
        int y = ui->backgroundProgressBar->y() - h;

        if (x < ui->backgroundProgressBar->x() - w) {
            x = ui->backgroundProgressBar->x() - w;
        } else if (x > endPointer->x() - w){
            x = endPointer->x() - w;
        }

        int pointingX = x + w, currentX = startPointer->x() + w;
        if (pointingX >= stepInPixels + currentX){
            x = startPointer->x() + stepInPixels * ((pointingX - currentX) / stepInPixels);

            setStartGeometry(x, y, w, h);
        } else if (pointingX <= currentX - stepInPixels){
            x = startPointer->x() - stepInPixels * ((currentX - pointingX) / stepInPixels);

            setStartGeometry(x, y, w, h);
        }

    } else if (endMoving) {
        int w = endPointer->width(), h = endPointer->height();
        int x = event->pos().x() - pointerOffset.x();
        int y = ui->backgroundProgressBar->y() - h;

        if (x > ui->backgroundProgressBar->x() + ui->backgroundProgressBar->width()) {
            x = ui->backgroundProgressBar->x() + ui->backgroundProgressBar->width();
        } else if (x < startPointer->x() + startPointer->width()){
            x = startPointer->x() + startPointer->width();
        }


        int pointingX = x, currentX = endPointer->x();
        if (pointingX >= stepInPixels + currentX){
            x = currentX + stepInPixels * ((pointingX - currentX) / stepInPixels);

            setEndGeometry(x, y, w, h);
        } else if (pointingX <= currentX - stepInPixels){
            x = currentX - stepInPixels * ((currentX - pointingX) / stepInPixels);

            setEndGeometry(x, y, w, h);
        }
    }
}
