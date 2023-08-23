#include "intervaldialog.h"
#include "ui_intervaldialog.h"

IntervalDialog::IntervalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntervalDialog)
{
    ui->setupUi(this);
    ui->retranslateUi(this);
    activeTimeAxis = ui->timeAxisUp;
    activeListWidget = ui->listWidgetUp;
    setFixedWidth(578);

    mioSuit = MioSuit::getMioSuitInstance();
    body = Body::getBodyInstance();

    connect(ui->timeAxisUp, SIGNAL(startChanged(int)), this, SLOT(onStartChanged(int)));
    connect(ui->timeAxisUp, SIGNAL(endChanged(int)), this, SLOT(onEndChanged(int)));
    connect(ui->timeAxisDown, SIGNAL(startChanged(int)), this, SLOT(onStartChanged(int)));
    connect(ui->timeAxisDown, SIGNAL(endChanged(int)), this, SLOT(onEndChanged(int)));

    connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(onAddClicked()));
    connect(ui->pushButtonDelete, SIGNAL(clicked()), this, SLOT(onDeleteClicked()));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(ui->pushButtonShow, SIGNAL(clicked()), this, SLOT(onShowClicked()));
    connect(ui->listWidgetUp, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListItemClicked(QListWidgetItem*)));
    connect(ui->listWidgetDown, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListItemClicked(QListWidgetItem*)));

    ui->spinBoxStart->setMaximum(MAX_INTERVAL);
    ui->spinBoxEnd->setMaximum(MAX_INTERVAL);
    connect(ui->spinBoxStart, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxStartChanged(int)));
    connect(ui->spinBoxEnd, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxEndChanged(int)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));

    ui->spinBoxEnd->setEnabled(false);
    ui->spinBoxStart->setEnabled(false);
    ui->pushButtonAdd->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);

    ui->listWidgetUp->show();
    ui->timeAxisUp->show();
    ui->listWidgetDown->hide();
    ui->timeAxisDown->hide();
}

IntervalDialog::~IntervalDialog()
{
    delete ui;
}

void IntervalDialog::addPairs(int count)
{
    QVector<IntervalDotsPair *> pairs = mioSuit->getIntervalDotsPairs(isUpActive());
    activeDotsPair = pairs.last();
    for (qsizetype i = pairs.size() - count; i < pairs.size(); i++) {
        activeListWidget->addItem(pairs[i]->getName());
        activeTimeAxis->addNewProgressBar();
    }
    activeListWidget->setCurrentRow(activeListWidget->count()-1);
    setModal(true);
    show();
}

void IntervalDialog::translate()
{
    ui->retranslateUi(this);
}

bool IntervalDialog::isUpActive()
{
    return activeMode == UP;
}

void IntervalDialog::updatePairs()
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

void IntervalDialog::setIsAdminModeActive(bool newIsAdminModeActive)
{
    isAdminModeActive = newIsAdminModeActive;
    ui->spinBoxEnd->setEnabled(newIsAdminModeActive);
    ui->spinBoxStart->setEnabled(newIsAdminModeActive);
    ui->pushButtonAdd->setEnabled(newIsAdminModeActive);
    ui->pushButtonDelete->setEnabled(newIsAdminModeActive);

    ui->timeAxisDown->setIsAdminModeActive(newIsAdminModeActive);
    ui->timeAxisUp->setIsAdminModeActive(newIsAdminModeActive);
}

void IntervalDialog::onAddClicked()
{
    if (!isAdminModeActive) return;
    activeDotsPair = nullptr;
    emit add();
    emit partChanged(ui->comboBox->currentIndex());
    hide();
    setModal(false);
}

void IntervalDialog::onDeleteClicked()
{
    if (!isAdminModeActive) return;
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
        if (mioSuit->getIntervalDotsPairs(ui->comboBox->currentIndex() == UP).size() != 0) {
            ui->listWidgetUp->setCurrentRow(ui->listWidgetUp->count() - 1);
            int index = activeListWidget->currentRow();
            activeDotsPair = mioSuit->getIntervalDotsPairAt(index, isUpActive());
            activeTimeAxis->selectProgressBar(index);
        }
    }
}

void IntervalDialog::onSaveClicked()
{
    hide();
    emit partChanged(2);
    setModal(false);
}

void IntervalDialog::onShowClicked()
{
    qDebug() << "Show";
    hide();
    IntervalSimulation dialog(isUpActive());
    dialog.exec();
    mioSuit->update();
    body->update();
    show();
}

void IntervalDialog::onStartChanged(int seconds)
{
    changedByUser = false;
    ui->spinBoxStart->setValue(seconds);
    if (activeDotsPair != nullptr)
        activeDotsPair->setStartTime(seconds);
}

void IntervalDialog::onEndChanged(int seconds)
{
    changedByUser = false;
    ui->spinBoxEnd->setValue(seconds);
    if (activeDotsPair != nullptr)
        activeDotsPair->setEndTime(seconds);
}

void IntervalDialog::onSpinBoxStartChanged(int seconds)
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

void IntervalDialog::onSpinBoxEndChanged(int seconds)
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

void IntervalDialog::onComboBoxIndexChanged(int index)
{
    if (index == UP) {
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
}

void IntervalDialog::onListItemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item)
    int index = activeListWidget->currentRow();
    activeDotsPair = mioSuit->getIntervalDotsPairAt(index, isUpActive());
    activeTimeAxis->selectProgressBar(index);
}
