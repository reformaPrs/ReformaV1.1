#include "intervalwidget.h"
#include "ui_intervalwidget.h"

IntervalWidget::IntervalWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntervalWidget)
{
    ui->setupUi(this);
    ui->retranslateUi(this);
    activeTimeAxis = ui->timeAxisUp;
    activeListWidget = ui->listWidgetUp;

    mioSuit = MioSuit::getMioSuitInstance();
    body = Body::getBodyInstance();

    selectionButtonGroup = new QButtonGroup(this);
    selectionButtonGroup->addButton(ui->toolButtonUp);
    selectionButtonGroup->addButton(ui->toolButtonDown);
    selectionButtonGroup->setExclusive(true);

    connect(ui->timeAxisUp, SIGNAL(startChanged(int)), this, SLOT(onStartChanged(int)));
    connect(ui->timeAxisUp, SIGNAL(endChanged(int)), this, SLOT(onEndChanged(int)));
    connect(ui->timeAxisDown, SIGNAL(startChanged(int)), this, SLOT(onStartChanged(int)));
    connect(ui->timeAxisDown, SIGNAL(endChanged(int)), this, SLOT(onEndChanged(int)));

    connect(ui->pushButtonDelete, SIGNAL(clicked()), this, SLOT(onDeleteClicked()));
    connect(ui->pushButtonShow, SIGNAL(clicked()), this, SLOT(onShowClicked()));
    connect(ui->listWidgetUp, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListItemClicked(QListWidgetItem*)));
    connect(ui->listWidgetDown, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListItemClicked(QListWidgetItem*)));

    ui->spinBoxStart->setMaximum(MAX_INTERVAL);
    ui->spinBoxEnd->setMaximum(MAX_INTERVAL);
    connect(ui->spinBoxStart, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxStartChanged(int)));
    connect(ui->spinBoxEnd, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxEndChanged(int)));
    connect(selectionButtonGroup, SIGNAL(buttonToggled(QAbstractButton*,bool)), this, SLOT(onSelectionButtonToggled(QAbstractButton*,bool)));
    if (!kIsProfVersion) {
        ui->spinBoxEnd->setEnabled(false);
        ui->spinBoxStart->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
    }

    ui->listWidgetUp->show();
    ui->timeAxisUp->show();
    ui->listWidgetDown->hide();
    ui->timeAxisDown->hide();
}

IntervalWidget::~IntervalWidget()
{
    delete ui;
}

void IntervalWidget::addPairs(int count)
{
    QVector<IntervalDotsPair *> pairs = mioSuit->getIntervalDotsPairs(isUpActive());
    activeDotsPair = pairs.last();
    for (qsizetype i = pairs.size() - count; i < pairs.size(); i++) {
        activeListWidget->addItem(pairs[i]->getName());
        activeTimeAxis->addNewProgressBar();
    }
    activeListWidget->setCurrentRow(activeListWidget->count()-1);
}

void IntervalWidget::translate()
{
    ui->retranslateUi(this);
}

bool IntervalWidget::isUpActive()
{
    return activeMode == UP;
}

void IntervalWidget::updatePairs()
{
    activeDotsPair = nullptr;
    for (qsizetype i = 0; i < ui->listWidgetUp->count(); i++) {
        ui->timeAxisUp->deleteProgressBar(0);
    }
    ui->listWidgetUp->clear();
    QVector<IntervalDotsPair *> intervalpairs = mioSuit->getIntervalDotsPairs(true);
    for (auto pair : intervalpairs) {
        ui->timeAxisUp->addNewProgressBar();
        ui->timeAxisUp->setStartTime(pair->getStartTime());
        ui->timeAxisUp->setEndTime(pair->getEndTime());
        ui->listWidgetUp->addItem(pair->getName());
    }

    if (mioSuit->getIntervalDotsPairs(true).size() != 0) {
        activeDotsPair = mioSuit->getIntervalDotsPairs(true).last();
        ui->listWidgetUp->setCurrentRow(ui->listWidgetUp->count() - 1);
    }

    for (qsizetype i = 0; i < ui->listWidgetDown->count(); i++) {
        ui->timeAxisDown->deleteProgressBar(0);
    }
    ui->listWidgetDown->clear();

    intervalpairs = mioSuit->getIntervalDotsPairs(false);
    for (auto pair : intervalpairs) {
        ui->timeAxisDown->addNewProgressBar();
        ui->timeAxisDown->setStartTime(pair->getStartTime());
        ui->timeAxisDown->setEndTime(pair->getEndTime());
        ui->listWidgetDown->addItem(pair->getName());
    }
}

void IntervalWidget::onDeleteClicked()
{
    if (!kIsProfVersion) return;
//    qDebug() << "Delete";
    QListWidgetItem *currentItem = activeListWidget->currentItem();
    if (currentItem != nullptr) {
        int index = activeListWidget->currentRow();
        int deletedPartID = activeDotsPair->getPartID();
        activeTimeAxis->deleteProgressBar(activeListWidget->currentRow());
        activeListWidget->removeItemWidget(currentItem);
        activeListWidget->update();
        delete currentItem;
        delete activeDotsPair;
        mioSuit->removeIntervalDotsPairAt(index, isUpActive());
        activeDotsPair = nullptr;

        if (!mioSuit->isIntervalDotsPairWithPartID(deletedPartID, isUpActive())) {
            body->setPartStatus(deletedPartID, AbstractBodyPart::BothDisabled);
        }
        if (mioSuit->getIntervalDotsPairs(isUpActive()).size() != 0) {
            ui->listWidgetUp->setCurrentRow(ui->listWidgetUp->count() - 1);
            int index = activeListWidget->currentRow();
            activeDotsPair = mioSuit->getIntervalDotsPairAt(index, isUpActive());
            activeTimeAxis->selectProgressBar(index);
        }
    }
}

void IntervalWidget::onShowClicked()
{
    qDebug() << "Show";
    hide();
    emit showClicked();
}

void IntervalWidget::onStartChanged(int seconds)
{
    changedByUser = false;
    ui->spinBoxStart->setValue(seconds);
    if (activeDotsPair != nullptr)
        activeDotsPair->setStartTime(seconds);
}

void IntervalWidget::onEndChanged(int seconds)
{
    changedByUser = false;
    ui->spinBoxEnd->setValue(seconds);
    if (activeDotsPair != nullptr)
        activeDotsPair->setEndTime(seconds);
}

void IntervalWidget::onSpinBoxStartChanged(int seconds)
{
    if (!changedByUser) {
        changedByUser = true;
        return;
    }
    changedByUser = false;
    activeTimeAxis->setStartTime(seconds);
    if (activeDotsPair != nullptr)
        activeDotsPair->setStartTime(seconds);
    changedByUser = true;
}

void IntervalWidget::onSpinBoxEndChanged(int seconds)
{
    if (!changedByUser) {
        changedByUser = true;
        return;
    }
    changedByUser = false;
    activeTimeAxis->setEndTime(seconds);
    if (activeDotsPair != nullptr)
        activeDotsPair->setEndTime(seconds);
    changedByUser = true;
}

void IntervalWidget::onListItemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item)
    int index = activeListWidget->currentRow();
    activeDotsPair = mioSuit->getIntervalDotsPairAt(index, isUpActive());
    activeTimeAxis->selectProgressBar(index);
}

void IntervalWidget::showEvent(QShowEvent *event)
{
    emit partChanged(ui->toolButtonUp->isChecked() ? 0 : 1);
    QWidget::showEvent(event);
}

void IntervalWidget::onSelectionButtonToggled(QAbstractButton *, bool checked){
    if (checked){
        if (ui->toolButtonUp->isChecked()){
            activeMode = UP;
            ui->listWidgetUp->show();
            ui->timeAxisUp->show();
            ui->listWidgetDown->hide();
            ui->timeAxisDown->hide();
            activeListWidget = ui->listWidgetUp;
            activeTimeAxis = ui->timeAxisUp;
        } else {
            activeMode = DOWN;
            ui->listWidgetUp->hide();
            ui->timeAxisUp->hide();
            ui->listWidgetDown->show();
            ui->timeAxisDown->show();
            activeListWidget = ui->listWidgetDown;
            activeTimeAxis = ui->timeAxisDown;
        }
        emit partChanged(ui->toolButtonUp->isChecked() ? 0 : 1);
    }
}
