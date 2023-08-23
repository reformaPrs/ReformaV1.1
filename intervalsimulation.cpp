#include "intervalsimulation.h"
#include "ui_intervalsimulation.h"

IntervalSimulation::IntervalSimulation(bool isUp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntervalSimulation),
    isUp(isUp)
{
    ui->setupUi(this);
    move(100, 100);
    setFixedSize(578, 250);

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

IntervalSimulation::~IntervalSimulation()
{
    delete timer;
    delete ui;
}

void IntervalSimulation::onOKClicked()
{
    accept();
}

void IntervalSimulation::onPlayClicked()
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

void IntervalSimulation::onTimerTimeout()
{
    ui->timeAxis->moveSimulation();
}

void IntervalSimulation::onSimulationEnd()
{
    isGoing = false;
    ui->toolButton->setIcon(QPixmap(":/icons/icons/play.png"));
    timer->stop();
}

void IntervalSimulation::onSimulationTimeChanged(int seconds)
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
