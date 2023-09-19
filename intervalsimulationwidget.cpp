#include "intervalsimulationwidget.h"
#include "ui_intervalsimulationwidget.h"

IntervalSimulationWidget::IntervalSimulationWidget (bool isUp, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntervalSimulationWidget),
    isUp(isUp)
{
    ui->setupUi(this);

    body = Body::getBodyInstance();
    body->saveHalfhour();

    mioSuit = MioSuit::getMioSuitInstance();
    mioSuit->saveHalfhour();

    for (auto pair : mioSuit->getIntervalDotsPairs(isUp)) {
        ui->timeAxis->addNewProgressBar();
        ui->timeAxis->setStartTime(pair->getStartTime());
        ui->timeAxis->setEndTime(pair->getEndTime());
        ui->listWidget->addItem(pair->getName());
    }
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
    ui->timeAxis->initSimulation();
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listWidget->setAttribute(Qt::WA_TransparentForMouseEvents);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onOKClicked()));
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(onPlayClicked()));
    connect(ui->timeAxis, SIGNAL(simulationEnded()), this, SLOT(onSimulationEnd()));
    connect(ui->timeAxis, SIGNAL(simulationTimeChanged(int)), this, SLOT(onSimulationTimeChanged(int)));
}

IntervalSimulationWidget::~IntervalSimulationWidget()
{
    delete timer;
    delete ui;
}

void IntervalSimulationWidget::onOKClicked()
{
    emit done();
    delete this;
}

void IntervalSimulationWidget::onPlayClicked()
{
    if (isGoing) {
        isGoing = false;
        ui->toolButton->setIcon(QPixmap(":/icons/icons/play.png"));
        timer->stop();
    } else {
        isGoing = true;
        ui->toolButton->setIcon(QPixmap(":/icons/icons/pause.png"));
        timer->start(ANIMATION_TICK);
    }
}

void IntervalSimulationWidget::onTimerTimeout()
{
    ui->timeAxis->moveSimulation();
}

void IntervalSimulationWidget::onSimulationEnd()
{
    isGoing = false;
    ui->toolButton->setIcon(QPixmap(":/icons/icons/play.png"));
    timer->stop();
}

void IntervalSimulationWidget::onSimulationTimeChanged(int seconds)
{
    ui->listWidget->clearSelection();
    body->resetIntervalParts();
    mioSuit->update();
    QVector<IntervalDotsPair *> dotsPairs = mioSuit->getIntervalDotsPairs(isUp);
    for (qsizetype i = 0; i < dotsPairs.size(); i++) {
        if (dotsPairs[i]->getStartTime() <= seconds && dotsPairs[i]->getEndTime() > seconds){
            ui->listWidget->setCurrentRow(i, QItemSelectionModel::Select);
            mioSuit->setDotsPairSimulationValue(dotsPairs[i]->getID(), dotsPairs[i]->getValue());
            body->setCalculatePartStatus(dotsPairs[i]->getPartID(), mioSuit->isFirstMuscleByID(dotsPairs[i]->getID()));
        }
    }
}
