#include "programwidget.h"
#include "qbuttongroup.h"
#include "ui_programwidget.h"

ProgramWidget::ProgramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgramWidget)
{
    ui->setupUi(this);
    firstHalfhourBtn = new QPushButton("Первые 30 минут", this);
    secondHalfhourBtn = new QPushButton("Вторые 30 минут", this);
    firstHalfhourBtn->setCheckable(true);
    secondHalfhourBtn->setCheckable(true);
    halfhourBtnsGroup = new QButtonGroup(this);
    halfhourBtnsGroup->addButton(firstHalfhourBtn);
    halfhourBtnsGroup->addButton(secondHalfhourBtn);
    halfhourBtnsGroup->setExclusive(true);

    firstHalfhourBtn->hide();
    secondHalfhourBtn->hide();

    connect(halfhourBtnsGroup, SIGNAL(buttonToggled(QAbstractButton*,bool)), this, SLOT(onHalfhourToggled(QAbstractButton*,bool)));

    connect(ui->showBtn, SIGNAL(clicked()), this, SLOT(onShowClicked()));

    connect(ui->checkBoxDexterSide, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxDexterAnt, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxDexterAntUp, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxDexterAntDown, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(ui->checkBoxSinisterSide, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxSinisterAnt, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxSinisterAntUp, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxSinisterAntDown, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(ui->checkBoxDexterSideAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxDexterAntAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxDexterAntUpAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxDexterAntDownAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(ui->checkBoxSinisterSideAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxSinisterAntAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxSinisterAntUpAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxSinisterAntDownAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));


    connect(ui->checkBoxNeck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->checkBoxNeckAdmin, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    adminMode(false);

    ui->showBtn->setMinimumHeight(20);
}

ProgramWidget::~ProgramWidget()
{
    delete ui;
}

void ProgramWidget::setIsHalfhourActive(bool newIsHalfhourActive)
{
    isHalfhourActive = newIsHalfhourActive;
    if (!newIsHalfhourActive){

        halfhourBtnsGroup->setExclusive(false);
        firstHalfhourBtn->setChecked(false);
        secondHalfhourBtn->setChecked(false);
        halfhourBtnsGroup->setExclusive(true);

        firstHalfhourBtn->hide();
        secondHalfhourBtn->hide();
    }
    else
    {
        firstHalfhourBtn->show();
        secondHalfhourBtn->show();
    }
}

bool ProgramWidget::getIsHalfhourActive() const
{
    return isHalfhourActive;
}

ProgramSlidersValues ProgramWidget::getProgramValues() const
{
    return programValues;
}

int ProgramWidget::getHalfhourIndex()
{
    if (firstHalfhourBtn->isChecked()){
        return 0;
    }
    return 1;
}

void ProgramWidget::setHalfHourIndex(int index)
{
    if (index == 0){
        firstHalfhourBtn->setChecked(true);
        secondHalfhourBtn->setChecked(false);
    } else {
        firstHalfhourBtn->setChecked(false);
        secondHalfhourBtn->setChecked(true);
    }
}

void ProgramWidget::clear()
{
    halfhourBtnsGroup->setExclusive(false);
    firstHalfhourBtn->setChecked(false);
    secondHalfhourBtn->setChecked(false);
    halfhourBtnsGroup->setExclusive(true);

    isHalfhourActive = false;

    ui->checkBoxDexterAnt->setChecked(false);
    ui->checkBoxDexterAntDown->setChecked(false);
    ui->checkBoxDexterAntUp->setChecked(false);
    ui->checkBoxDexterSide->setChecked(false);

    ui->checkBoxSinisterSide->setChecked(false);
    ui->checkBoxSinisterAnt->setChecked(false);
    ui->checkBoxSinisterAntDown->setChecked(false);
    ui->checkBoxSinisterAntUp->setChecked(false);
    ui->checkBoxNeck->setChecked(false);

    ui->checkBoxDexterAntAdmin->setChecked(false);
    ui->checkBoxDexterAntDownAdmin->setChecked(false);
    ui->checkBoxDexterAntUpAdmin->setChecked(false);
    ui->checkBoxDexterSideAdmin->setChecked(false);

    ui->checkBoxSinisterSideAdmin->setChecked(false);
    ui->checkBoxSinisterAntAdmin->setChecked(false);
    ui->checkBoxSinisterAntDownAdmin->setChecked(false);
    ui->checkBoxSinisterAntUpAdmin->setChecked(false);
    ui->checkBoxNeckAdmin->setChecked(false);


    ui->labeledSliderSinisterUp1->setValue(0);
    ui->labeledSliderSinisterUp2->setValue(0);
    ui->labeledSliderSinisterDown1->setValue(0);
    ui->labeledSliderSinisterDown2->setValue(0);
    ui->labeledSliderDexterUp1->setValue(0);
    ui->labeledSliderDexterUp2->setValue(0);
    ui->labeledSliderDexterDown1->setValue(0);
    ui->labeledSliderDexterDown2->setValue(0);
    ui->labeledSliderNeck->setValue(0);

    for(int index = 0; index < 2; index++){
        slidersValues.setUpDexterValue(index, ui->labeledSliderDexterUp1->getValue());
        slidersValues.setUpDexterAntValue(index, ui->labeledSliderDexterUp2->getValue());
        slidersValues.setDownDexterValue(index, ui->labeledSliderDexterDown1->getValue());
        slidersValues.setDownDexterAntValue(index,ui->labeledSliderDexterDown2->getValue());

        slidersValues.setUpSinisterValue(index, ui->labeledSliderSinisterUp1->getValue());
        slidersValues.setUpSinisterAntValue(index, ui->labeledSliderSinisterUp2->getValue());
        slidersValues.setDownSinisterValue(index,  ui->labeledSliderSinisterDown1->getValue());
        slidersValues.setDownSinisterAntValue(index, ui->labeledSliderSinisterDown2->getValue());
        slidersValues.setNeckValue(index, ui->labeledSliderNeck->getValue());
        setProgramValues(index);
    }
}

void ProgramWidget::setSlidersBoundries(ProgramSlidersValues minValues, ProgramSlidersValues maxValues)
{
    slidersMinValues = minValues;
    slidersMaxValues = maxValues;
    if (!isHalfhourActive) {
        setHalfhourSlidersBoundries(0);
        setHalfhourSlidersBoundries(1);

        setProgramValues(0);
        setProgramValues(1);
        return;
    }
    if (firstHalfhourBtn->isChecked()) {
        setHalfHourIndex(0);
        setProgramValues(0);
    } else {
        setHalfHourIndex(1);
        setProgramValues(1);
    }
}

void ProgramWidget::setHalfhourSlidersBoundries(int index){
    ui->labeledSliderDexterUp1->setMin(slidersMinValues.getUpDexterValue(index));
    ui->labeledSliderDexterUp2->setMin(slidersMinValues.getUpDexterAntValue(index));
    ui->labeledSliderDexterDown1->setMin(slidersMinValues.getDownDexterValue(index));
    ui->labeledSliderDexterDown2->setMin(slidersMinValues.getDownDexterAntValue(index));
    ui->labeledSliderSinisterUp1->setMin(slidersMinValues.getUpSinisterValue(index));
    ui->labeledSliderSinisterUp2->setMin(slidersMinValues.getUpSinisterAntValue(index));
    ui->labeledSliderSinisterDown1->setMin(slidersMinValues.getDownSinisterValue(index));
    ui->labeledSliderSinisterDown2->setMin(slidersMinValues.getDownSinisterAntValue(index));
    ui->labeledSliderNeck->setMin(slidersMinValues.getNeckValue(index));

    ui->labeledSliderDexterUp1->setMax(slidersMaxValues.getUpDexterValue(index));
    ui->labeledSliderDexterUp2->setMax(slidersMaxValues.getUpDexterAntValue(index));
    ui->labeledSliderDexterDown1->setMax(slidersMaxValues.getDownDexterValue(index));
    ui->labeledSliderDexterDown2->setMax(slidersMaxValues.getDownDexterAntValue(index));
    ui->labeledSliderSinisterUp1->setMax(slidersMaxValues.getUpSinisterValue(index));
    ui->labeledSliderSinisterUp2->setMax(slidersMaxValues.getUpSinisterAntValue(index));
    ui->labeledSliderSinisterDown1->setMax(slidersMaxValues.getDownSinisterValue(index));
    ui->labeledSliderSinisterDown2->setMax(slidersMaxValues.getDownSinisterAntValue(index));
    ui->labeledSliderNeck->setMax(slidersMaxValues.getNeckValue(index));
}

void ProgramWidget::setProgramValues(int index)
{
    programValues.setUpDexterValue(index, ui->labeledSliderDexterUp1->getValue());
    programValues.setUpDexterAntValue(index, ui->labeledSliderDexterUp2->getValue());
    programValues.setDownDexterValue(index, ui->labeledSliderDexterDown1->getValue());
    programValues.setDownDexterAntValue(index,ui->labeledSliderDexterDown2->getValue());

    programValues.setUpSinisterValue(index, ui->labeledSliderSinisterUp1->getValue());
    programValues.setUpSinisterAntValue(index, ui->labeledSliderSinisterUp2->getValue());
    programValues.setDownSinisterValue(index,  ui->labeledSliderSinisterDown1->getValue());
    programValues.setDownSinisterAntValue(index, ui->labeledSliderSinisterDown2->getValue());

    programValues.setNeckValue(index, ui->labeledSliderNeck->getValue());

    /*if (ui->labeledSliderDexterUp1->isEnabled()) {
        programValues.setUpDexterValue(index, ui->labeledSliderDexterUp1->getValue());
    } else {
        programValues.setUpDexterValue(index, 0);
    }

    if (ui->labeledSliderDexterUp2->isEnabled()){
        programValues.setUpDexterAntValue(index, ui->labeledSliderDexterUp2->getValue());
    } else {
        programValues.setUpDexterAntValue(index, 0);
    }

    if (ui->labeledSliderDexterDown1->isEnabled()){
        programValues.setDownDexterValue(index, ui->labeledSliderDexterDown1->getValue());
    } else {
        programValues.setDownDexterValue(index, 0);
    }


    if (ui->labeledSliderDexterDown2->isEnabled()){
        programValues.setDownDexterAntValue(index,ui->labeledSliderDexterDown2->getValue());
    } else {
        programValues.setDownDexterAntValue(index, 0);
    }


    if (ui->labeledSliderSinisterUp1->isEnabled()){
        programValues.setUpSinisterValue(index, ui->labeledSliderSinisterUp1->getValue());
    } else {
        programValues.setUpSinisterValue(index, 0);
    }

    if (ui->labeledSliderSinisterUp2->isEnabled()){
        programValues.setUpSinisterAntValue(index, ui->labeledSliderSinisterUp2->getValue());
    } else {
        programValues.setUpSinisterAntValue(index, 0);
    }

    if (ui->labeledSliderSinisterDown1->isEnabled()){
        programValues.setDownSinisterValue(index,  ui->labeledSliderSinisterDown1->getValue());
    } else {
        programValues.setDownSinisterValue(index, 0);
    }

    if (ui->labeledSliderSinisterDown2->isEnabled()){
        programValues.setDownSinisterAntValue(index, ui->labeledSliderSinisterDown2->getValue());
    } else {
        programValues.setDownSinisterAntValue(index, 0);
    }

    if (ui->labeledSliderNeck->isEnabled()){
        programValues.setNeckValue(index, (ui->labeledSliderNeck->getValue()));
    } else {
        programValues.setNeckValue(index, 0);
    }*/
}

void ProgramWidget::saveHalfHour(int index)
{
    isCheckBoxChecked[index][0] = ui->checkBoxDexterSide->isChecked();
    isCheckBoxChecked[index][1] = ui->checkBoxDexterAnt->isChecked();
    isCheckBoxChecked[index][2] = ui->checkBoxDexterAntUp->isChecked();
    isCheckBoxChecked[index][3] = ui->checkBoxDexterAntDown->isChecked();
    isCheckBoxChecked[index][4] = ui->checkBoxSinisterSide->isChecked();
    isCheckBoxChecked[index][5] = ui->checkBoxSinisterAnt->isChecked();
    isCheckBoxChecked[index][6] = ui->checkBoxSinisterAntUp->isChecked();
    isCheckBoxChecked[index][7] = ui->checkBoxSinisterAntDown->isChecked();
    isCheckBoxChecked[index][8] = ui->checkBoxNeck->isChecked();
    setProgramValues(index);

    slidersValues.setUpDexterValue(index, ui->labeledSliderDexterUp1->getValue());
    slidersValues.setUpDexterAntValue(index, ui->labeledSliderDexterUp2->getValue());
    slidersValues.setDownDexterValue(index, ui->labeledSliderDexterDown1->getValue());
    slidersValues.setDownDexterAntValue(index,ui->labeledSliderDexterDown2->getValue());

    slidersValues.setUpSinisterValue(index, ui->labeledSliderSinisterUp1->getValue());
    slidersValues.setUpSinisterAntValue(index, ui->labeledSliderSinisterUp2->getValue());
    slidersValues.setDownSinisterValue(index,  ui->labeledSliderSinisterDown1->getValue());
    slidersValues.setDownSinisterAntValue(index, ui->labeledSliderSinisterDown2->getValue());
    slidersValues.setNeckValue(index, ui->labeledSliderNeck->getValue());
}

void ProgramWidget::setHalfhourSlidersValue(int index){
    ui->labeledSliderDexterUp1->setValue(slidersValues.getUpDexterValue(index));
    ui->labeledSliderDexterUp2->setValue(slidersValues.getUpDexterAntValue(index));
    ui->labeledSliderDexterDown1->setValue(slidersValues.getDownDexterValue(index));
    ui->labeledSliderDexterDown2->setValue(slidersValues.getDownDexterAntValue(index));
    ui->labeledSliderSinisterUp1->setValue(slidersValues.getUpSinisterValue(index));
    ui->labeledSliderSinisterUp2->setValue(slidersValues.getUpSinisterAntValue(index));
    ui->labeledSliderSinisterDown1->setValue(slidersValues.getDownSinisterValue(index));
    ui->labeledSliderSinisterDown2->setValue(slidersValues.getDownSinisterAntValue(index));
    ui->labeledSliderNeck->setValue(slidersValues.getNeckValue(index));
}

void ProgramWidget::setSlidersValues(const ProgramSlidersValues& values){
    slidersValues = values;
    if (firstHalfhourBtn->isChecked()){
        setHalfhourSlidersValue(0);
    } else {
        setHalfhourSlidersValue(1);
    }
}

void ProgramWidget::adminMode(bool isActive)
{

    ui->checkBoxDexterAntAdmin->setVisible(isActive);
    ui->checkBoxDexterAntDownAdmin->setVisible(isActive);
    ui->checkBoxDexterAntUpAdmin->setVisible(isActive);
    ui->checkBoxDexterSideAdmin->setVisible(isActive);

    ui->checkBoxSinisterSideAdmin->setVisible(isActive);
    ui->checkBoxSinisterAntAdmin->setVisible(isActive);
    ui->checkBoxSinisterAntDownAdmin->setVisible(isActive);
    ui->checkBoxSinisterAntUpAdmin->setVisible(isActive);

    ui->checkBoxNeckAdmin->setVisible(isActive);
    //ui->showBtn->setVisible(isActive);
}

void ProgramWidget::saveToFile(QDataStream *out)
{
    //Admin checkBoxes
    *out << ui->checkBoxDexterSideAdmin->isChecked();
    *out << ui->checkBoxDexterAntAdmin->isChecked();
    *out << ui->checkBoxDexterAntUpAdmin->isChecked();
    *out << ui->checkBoxDexterAntDownAdmin->isChecked();

    *out << ui->checkBoxSinisterSideAdmin->isChecked();
    *out << ui->checkBoxSinisterAntAdmin->isChecked();
    *out << ui->checkBoxSinisterAntUpAdmin->isChecked();
    *out << ui->checkBoxSinisterAntDownAdmin->isChecked();

    *out << ui->checkBoxNeckAdmin->isChecked();
    //User checkBoxes
    *out << ui->checkBoxDexterSide->isChecked();
    *out << ui->checkBoxDexterAnt->isChecked();
    *out << ui->checkBoxDexterAntUp->isChecked();
    *out << ui->checkBoxDexterAntDown->isChecked();

    *out << ui->checkBoxSinisterSide->isChecked();
    *out << ui->checkBoxSinisterAnt->isChecked();
    *out << ui->checkBoxSinisterAntUp->isChecked();
    *out << ui->checkBoxSinisterAntDown->isChecked();

    *out << ui->checkBoxNeck->isChecked();
}

void ProgramWidget::readFromFile(QDataStream *in)
{

    bool input;
    //Admin checkBoxes
    *in >> input;
    ui->checkBoxDexterSideAdmin->setChecked(input);
    *in >> input;
    ui->checkBoxDexterAntAdmin->setChecked(input);
    *in >> input;
    ui->checkBoxDexterAntUpAdmin->setChecked(input);
    *in >> input;
    ui->checkBoxDexterAntDownAdmin->setChecked(input);

    *in >> input;
    ui->checkBoxSinisterSideAdmin->setChecked(input);
    *in >> input;
    ui->checkBoxSinisterAntAdmin->setChecked(input);
    *in >> input;
    ui->checkBoxSinisterAntUpAdmin->setChecked(input);
    *in >> input;
    ui->checkBoxSinisterAntDownAdmin->setChecked(input);

    *in >> input;
    ui->checkBoxNeckAdmin->setChecked(input);

    //user checkBoxes
    *in >> input;
    ui->checkBoxDexterSide->setChecked(input);
    *in >> input;
    ui->checkBoxDexterAnt->setChecked(input);
    *in >> input;
    ui->checkBoxDexterAntUp->setChecked(input);
    *in >> input;
    ui->checkBoxDexterAntDown->setChecked(input);

    *in >> input;
    ui->checkBoxSinisterSide->setChecked(input);
    *in >> input;
    ui->checkBoxSinisterAnt->setChecked(input);
    *in >> input;
    ui->checkBoxSinisterAntUp->setChecked(input);
    *in >> input;
    ui->checkBoxSinisterAntDown->setChecked(input);

    *in >> input;
    ui->checkBoxNeck->setChecked(input);

    ui->labeledSliderDexterUp2->setEnabled(ui->checkBoxDexterAntUp->isChecked());

    ui->labeledSliderDexterDown2->setEnabled(ui->checkBoxDexterAntDown->isChecked());

    if (ui->checkBoxDexterAnt->isChecked()){
        ui->labeledSliderDexterUp2->setEnabled(true);
        ui->labeledSliderDexterDown2->setEnabled(true);

        ui->checkBoxDexterAntUp->setEnabled(false);
        ui->checkBoxDexterAntDown->setEnabled(false);

    } else {
        if (!ui->checkBoxDexterAntUpAdmin->isChecked()) {
            ui->checkBoxDexterAntUp->setEnabled(true);
        }
        if (!ui->checkBoxDexterAntDownAdmin->isChecked()) {
            ui->checkBoxDexterAntDown->setEnabled(true);
        }
        if (!ui->checkBoxDexterAntUp->isChecked()){
            ui->labeledSliderDexterUp2->setEnabled(false);
        }
        if (!ui->checkBoxDexterAntDown->isChecked()){
            ui->labeledSliderDexterDown2->setEnabled(false);
        }
    }

    if (ui->checkBoxDexterSide->isChecked()) {
        if (!ui->checkBoxDexterAnt->isChecked()) {
            if (!ui->checkBoxDexterAntUpAdmin->isChecked()) {
                ui->checkBoxDexterAntUp->setEnabled(true);
            }
            if (!ui->checkBoxDexterAntDownAdmin->isChecked()) {
                ui->checkBoxDexterAntDown->setEnabled(true);
            }
            if (ui->checkBoxDexterAntUp->isChecked()){
                ui->labeledSliderDexterUp2->setEnabled(true);
            }
            if (ui->checkBoxDexterAntDown->isChecked()){
                ui->labeledSliderDexterDown2->setEnabled(true);
            }
        } else {
            ui->labeledSliderDexterUp2->setEnabled(true);
            ui->labeledSliderDexterDown2->setEnabled(true);
        }

        if (!ui->checkBoxDexterAntAdmin->isChecked()){
            ui->checkBoxDexterAnt->setEnabled(true);
        }

        ui->labeledSliderDexterUp1->setEnabled(true);

        ui->labeledSliderDexterDown1->setEnabled(true);
    } else {
        //ui->checkBoxDexterAnt->setEnabled(false);
        //ui->checkBoxDexterAntUp->setEnabled(false);
        //ui->checkBoxDexterAntDown->setEnabled(false);
        ui->labeledSliderDexterUp1->setEnabled(false);
        //ui->labeledSliderDexterUp2->setEnabled(false);
        ui->labeledSliderDexterDown1->setEnabled(false);
        //ui->labeledSliderDexterDown2->setEnabled(false);
    }

    ui->labeledSliderSinisterUp2->setEnabled(ui->checkBoxSinisterAntUp->isChecked());

    ui->labeledSliderSinisterDown2->setEnabled(ui->checkBoxSinisterAntDown->isChecked());

    if (ui->checkBoxSinisterAnt->isChecked()){
        ui->labeledSliderSinisterUp2->setEnabled(true);
        ui->labeledSliderSinisterDown2->setEnabled(true);

        ui->checkBoxSinisterAntUp->setEnabled(false);
        ui->checkBoxSinisterAntDown->setEnabled(false);
    } else {
        if (!ui->checkBoxSinisterAntUpAdmin->isChecked()) {
            ui->checkBoxSinisterAntUp->setEnabled(true);
        }
        if (!ui->checkBoxSinisterAntDownAdmin->isChecked()) {
            ui->checkBoxSinisterAntDown->setEnabled(true);
        }
        if (!ui->checkBoxSinisterAntUp->isChecked()){
            ui->labeledSliderSinisterUp2->setEnabled(false);
        }
        if (!ui->checkBoxSinisterAntDown->isChecked()){
            ui->labeledSliderSinisterDown2->setEnabled(false);
        }
    }

    if (ui->checkBoxSinisterSide->isChecked()){
        if (!ui->checkBoxSinisterAnt->isChecked()){
            if (!ui->checkBoxSinisterAntUpAdmin->isChecked()) {
                ui->checkBoxSinisterAntUp->setEnabled(true);
            }
            if (!ui->checkBoxSinisterAntDownAdmin->isChecked()) {
                ui->checkBoxSinisterAntDown->setEnabled(true);
            }
            if (ui->checkBoxSinisterAntUp->isChecked()){
                ui->labeledSliderSinisterUp2->setEnabled(true);
            }
            if (ui->checkBoxSinisterAntDown->isChecked()){
                ui->labeledSliderSinisterDown2->setEnabled(true);
            }
        } else {
            ui->labeledSliderSinisterUp2->setEnabled(true);
            ui->labeledSliderSinisterDown2->setEnabled(true);
        }
        if (!ui->checkBoxSinisterAntAdmin->isChecked()) {
            ui->checkBoxSinisterAnt->setEnabled(true);
        }

        ui->labeledSliderSinisterUp1->setEnabled(true);

        ui->labeledSliderSinisterDown1->setEnabled(true);
    } else {
        //ui->checkBoxSinisterAnt->setEnabled(false);
        //ui->checkBoxSinisterAntUp->setEnabled(false);
        //ui->checkBoxSinisterAntDown->setEnabled(false);
        ui->labeledSliderSinisterUp1->setEnabled(false);
        //ui->labeledSliderSinisterUp2->setEnabled(false);
        ui->labeledSliderSinisterDown1->setEnabled(false);
        //ui->labeledSliderSinisterDown2->setEnabled(false);
    }

    ui->labeledSliderNeck->setEnabled(ui->checkBoxNeck->isChecked());

    //Admin checkboxes

    ui->checkBoxDexterSide->setEnabled(!ui->checkBoxDexterSideAdmin->isChecked());

    if (ui->checkBoxDexterSide->isChecked()) {
        ui->checkBoxDexterAnt->setEnabled(!ui->checkBoxDexterAntAdmin->isChecked());
    }

    if (!ui->checkBoxDexterAnt->isChecked() && ui->checkBoxDexterSide->isChecked()) {
        ui->checkBoxDexterAntUp->setEnabled(!ui->checkBoxDexterAntUpAdmin->isChecked());
    }

    if (!ui->checkBoxDexterAnt->isChecked() && ui->checkBoxDexterSide->isChecked()) {
        ui->checkBoxDexterAntDown->setEnabled(!ui->checkBoxDexterAntDownAdmin->isChecked());
    }


    ui->checkBoxSinisterSide->setEnabled(!ui->checkBoxSinisterSideAdmin->isChecked());

    if (ui->checkBoxSinisterSide->isChecked()) {
        ui->checkBoxSinisterAnt->setEnabled(!ui->checkBoxSinisterAntAdmin->isChecked());
    }

    if (!ui->checkBoxSinisterAnt->isChecked() && ui->checkBoxSinisterSide->isChecked()) {
        ui->checkBoxSinisterAntUp->setEnabled(!ui->checkBoxSinisterAntUpAdmin->isChecked());
    }

    if (!ui->checkBoxSinisterAnt->isChecked() && ui->checkBoxSinisterSide->isChecked()) {
        ui->checkBoxSinisterAntDown->setEnabled(!ui->checkBoxSinisterAntDownAdmin->isChecked());
    }

    ui->checkBoxNeck->setEnabled(!ui->checkBoxNeckAdmin->isChecked());
}

void ProgramWidget::createConfigData(QDataStream *out)
{
    for (int i = 0; i < 18; i++){
        *out << false;
    }
}

void ProgramWidget::translate()
{
    ui->retranslateUi(this);
}

void ProgramWidget::setHalfhour(int index)
{
    ui->checkBoxDexterSide->setChecked(isCheckBoxChecked[index][0]);
    ui->checkBoxDexterAnt->setChecked(isCheckBoxChecked[index][1]);
    ui->checkBoxDexterAntUp->setChecked(isCheckBoxChecked[index][2]);
    ui->checkBoxDexterAntDown->setChecked(isCheckBoxChecked[index][3]);
    ui->checkBoxSinisterSide->setChecked(isCheckBoxChecked[index][4]);
    ui->checkBoxSinisterAnt->setChecked(isCheckBoxChecked[index][5]);
    ui->checkBoxSinisterAntUp->setChecked(isCheckBoxChecked[index][6]);
    ui->checkBoxSinisterAntDown->setChecked(isCheckBoxChecked[index][7]);
    ui->checkBoxNeck->setChecked(isCheckBoxChecked[index][8]);

    setHalfhourSlidersValue(index);
}

void ProgramWidget::onShowClicked()
{
    if (isHalfhourActive){
        int index = 0;
        if (!firstHalfhourBtn->isChecked()){
            index = 1;
        }
        setProgramValues(index);
    } else {
        setProgramValues(0);
        setProgramValues(1);
    }
    emit showClicked();
}

void ProgramWidget::onCheckBoxStateChanged(int state)
{
    if (sender() == ui->checkBoxDexterSide) {
        if (state == 0){
            //ui->checkBoxDexterAnt->setEnabled(false);
            //ui->checkBoxDexterAntUp->setEnabled(false);
            //ui->checkBoxDexterAntDown->setEnabled(false);
            ui->labeledSliderDexterUp1->setEnabled(false);
            //ui->labeledSliderDexterUp2->setEnabled(false);
            ui->labeledSliderDexterDown1->setEnabled(false);
            //ui->labeledSliderDexterDown2->setEnabled(false);
        } else {
            if (!ui->checkBoxDexterAnt->isChecked()) {
                if (!ui->checkBoxDexterAntUpAdmin->isChecked()) {
                    ui->checkBoxDexterAntUp->setEnabled(true);
                }
                if (!ui->checkBoxDexterAntDownAdmin->isChecked()) {
                    ui->checkBoxDexterAntDown->setEnabled(true);
                }
                if (ui->checkBoxDexterAntUp->isChecked()){
                    ui->labeledSliderDexterUp2->setEnabled(true);
                }
                if (ui->checkBoxDexterAntDown->isChecked()){
                    ui->labeledSliderDexterDown2->setEnabled(true);
                }
            } else {
                ui->labeledSliderDexterUp2->setEnabled(true);
                ui->labeledSliderDexterDown2->setEnabled(true);
            }

            if (!ui->checkBoxDexterAntAdmin->isChecked()){
                ui->checkBoxDexterAnt->setEnabled(true);
            }

            ui->labeledSliderDexterUp1->setEnabled(true);

            ui->labeledSliderDexterDown1->setEnabled(true);
        }
        return;
    }

    if (sender() == ui->checkBoxDexterAnt){
        if(state == 0){
            if (!ui->checkBoxDexterAntUpAdmin->isChecked()) {
                ui->checkBoxDexterAntUp->setEnabled(true);
            }
            if (!ui->checkBoxDexterAntDownAdmin->isChecked()) {
                ui->checkBoxDexterAntDown->setEnabled(true);
            }
            if (!ui->checkBoxDexterAntUp->isChecked()){
                ui->labeledSliderDexterUp2->setEnabled(false);
            }
            if (!ui->checkBoxDexterAntDown->isChecked()){
                ui->labeledSliderDexterDown2->setEnabled(false);
            }
        }else{
            ui->labeledSliderDexterUp2->setEnabled(true);
            ui->labeledSliderDexterDown2->setEnabled(true);

            ui->checkBoxDexterAntUp->setEnabled(false);
            ui->checkBoxDexterAntDown->setEnabled(false);
        }
        return;
    }

    if (sender() == ui->checkBoxDexterAntUp){
        ui->labeledSliderDexterUp2->setEnabled(state);
        return;
    }

    if (sender() == ui->checkBoxDexterAntDown){
        ui->labeledSliderDexterDown2->setEnabled(state);
        return;
    }

    if (sender() == ui->checkBoxSinisterSide){
        if (state == 0){
            //ui->checkBoxSinisterAnt->setEnabled(false);
            //ui->checkBoxSinisterAntUp->setEnabled(false);
            //ui->checkBoxSinisterAntDown->setEnabled(false);
            ui->labeledSliderSinisterUp1->setEnabled(false);
            //ui->labeledSliderSinisterUp2->setEnabled(false);
            ui->labeledSliderSinisterDown1->setEnabled(false);
            //ui->labeledSliderSinisterDown2->setEnabled(false);
        } else {
            if (!ui->checkBoxSinisterAnt->isChecked()){
                if (!ui->checkBoxSinisterAntUpAdmin->isChecked()) {
                    ui->checkBoxSinisterAntUp->setEnabled(true);
                }
                if (!ui->checkBoxSinisterAntDownAdmin->isChecked()) {
                    ui->checkBoxSinisterAntDown->setEnabled(true);
                }
                if (ui->checkBoxSinisterAntUp->isChecked()){
                    ui->labeledSliderSinisterUp2->setEnabled(true);
                }
                if (ui->checkBoxSinisterAntDown->isChecked()){
                    ui->labeledSliderSinisterDown2->setEnabled(true);
                }
            } else {
                ui->labeledSliderSinisterUp2->setEnabled(true);
                ui->labeledSliderSinisterDown2->setEnabled(true);
            }
            if (!ui->checkBoxSinisterAntAdmin->isChecked()) {
                ui->checkBoxSinisterAnt->setEnabled(true);
            }

            ui->labeledSliderSinisterUp1->setEnabled(true);

            ui->labeledSliderSinisterDown1->setEnabled(true);
        }
        return;
    }

    if (sender() == ui->checkBoxSinisterAnt){
        if(state == 0){
            if (!ui->checkBoxSinisterAntUpAdmin->isChecked()) {
                ui->checkBoxSinisterAntUp->setEnabled(true);
            }
            if (!ui->checkBoxSinisterAntDownAdmin->isChecked()) {
                ui->checkBoxSinisterAntDown->setEnabled(true);
            }
            if (!ui->checkBoxSinisterAntUp->isChecked()){
                ui->labeledSliderSinisterUp2->setEnabled(false);
            }
            if (!ui->checkBoxSinisterAntDown->isChecked()){
                ui->labeledSliderSinisterDown2->setEnabled(false);
            }
        }else{
            ui->labeledSliderSinisterUp2->setEnabled(true);
            ui->labeledSliderSinisterDown2->setEnabled(true);

            ui->checkBoxSinisterAntUp->setEnabled(false);
            ui->checkBoxSinisterAntDown->setEnabled(false);
        }
        return;
    }

    if (sender() == ui->checkBoxSinisterAntUp){
        ui->labeledSliderSinisterUp2->setEnabled(state);
        return;
    }

    if (sender() == ui->checkBoxSinisterAntDown){
        ui->labeledSliderSinisterDown2->setEnabled(state);
        return;
    }

    if (sender() == ui->checkBoxNeck){
        ui->labeledSliderNeck->setEnabled(state);
    }


    //Admin checkboxes
    if (sender() == ui->checkBoxDexterSideAdmin){
        qDebug() << state;
        ui->checkBoxDexterSide->setEnabled(!state);
        return;
    }

    if (sender() == ui->checkBoxDexterAntAdmin){
        qDebug() << "ant";
        if (ui->checkBoxDexterSide->isChecked()) {
            ui->checkBoxDexterAnt->setEnabled(!state);
        }
        return;
    }

    if (sender() == ui->checkBoxDexterAntUpAdmin){
        qDebug() << "up";
        if (!ui->checkBoxDexterAnt->isChecked() && ui->checkBoxDexterSide->isChecked()) {
            ui->checkBoxDexterAntUp->setEnabled(!state);
        }
        return;
    }

    if (sender() == ui->checkBoxDexterAntDownAdmin){
        qDebug() << "down";
        if (!ui->checkBoxDexterAnt->isChecked() && ui->checkBoxDexterSide->isChecked()) {
            ui->checkBoxDexterAntDown->setEnabled(!state);
        }
        return;
    }

    if (sender() == ui->checkBoxSinisterSideAdmin){
        qDebug() << state;
        ui->checkBoxSinisterSide->setEnabled(!state);
        return;
    }

    if (sender() == ui->checkBoxSinisterAntAdmin){
        qDebug() << "ant";
        if (ui->checkBoxSinisterSide->isChecked()) {
            ui->checkBoxSinisterAnt->setEnabled(!state);
        }
        return;
    }

    if (sender() == ui->checkBoxSinisterAntUpAdmin){
        qDebug() << "up";
        if (!ui->checkBoxSinisterAnt->isChecked() && ui->checkBoxSinisterSide->isChecked()) {
            ui->checkBoxSinisterAntUp->setEnabled(!state);
        }
        return;
    }

    if (sender() == ui->checkBoxSinisterAntDownAdmin){
        qDebug() << "down";
        if (!ui->checkBoxSinisterAnt->isChecked() && ui->checkBoxSinisterSide->isChecked()) {
            ui->checkBoxSinisterAntDown->setEnabled(!state);
        }
        return;
    }

    if (sender() == ui->checkBoxNeckAdmin){
        qDebug() << state;
        ui->checkBoxNeck->setEnabled(!state);
        return;
    }
}

void ProgramWidget::onHalfhourToggled(QAbstractButton *, bool checked)
{
    if (checked) {
        if (!isHalfhourActive){
            isHalfhourActive = true;
            saveHalfHour(0);
            saveHalfHour(1);
            return;
        }
        if (firstHalfhourBtn->isChecked()){
            saveHalfHour(1);
            setHalfhour(0);
            setHalfhourSlidersBoundries(0);
        } else {
            saveHalfHour(0);
            setHalfhour(1);
            setHalfhourSlidersBoundries(1);
        }
    }
}

void ProgramWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    this->setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
    this->setContentsMargins(0, 0, 0, 30);

//    int h = 24;
//    int y = ui->showBtn->geometry().y()+ui->widget_3->geometry().y();
//    int x1 = 10;
//    int x2 = 120;
//    int w = 110;
//    firstHalfhourBtn->setGeometry(x1, y, w, h);
//    secondHalfhourBtn->setGeometry(x2, y, w, h);
    //    QWidget::resizeEvent(event);
}

void ProgramWidget::paintEvent(QPaintEvent *event)
{
//    QPixmap pixmap(width(), height());
//    pixmap.fill(Qt::white);
//    QPainter painter(this);
//    painter.drawPixmap(0, 0, pixmap);

    QWidget::paintEvent(event);

    QPainter painter(this);

    QPixmap background(":/backgrounds/backgrounds/programWidgetBackground.svg");

    painter.drawPixmap(rect(), background);

    this->setStyleSheet("QWidget"
                        "{"
                        "color:white;"
                        "font-size: 14px;"
                        "font-family: Sans Serif;"
                        "}");
    ui->showBtn->setStyleSheet("QPushButton"
                               "{"
                               "color: black;"
                               "}");

    QList<QCheckBox*> checkBoxes = findChildren<QCheckBox*>();
    for(QCheckBox* checkBox: checkBoxes){
        checkBox->setStyleSheet("QCheckBox"
                                "{"
                                "spacing: 5px;"
                                "}"
                                "QCheckBox::indicator"
                                "{"
                                "width : 20px;"
                                "height : 20px;"
                                "}"
                                "QCheckBox::indicator:unchecked"
                                "{"
                                "border-image: url(:/backgrounds/backgrounds/checkBoxGrayBackground.svg) 0 0 0 0 stretch stretch;"
                                "}"
                                "QCheckBox::indicator:checked"
                                "{"
                                "border-image: url(:/backgrounds/backgrounds/checkBoxRedBackground.svg) 0 0 0 0 stretch stretch;"
                                "}"
                                "QCheckBox::indicator:hover:unchecked"
                                "{"
                                "border-image: url(:/backgrounds/backgrounds/checkBoxBlueBackground.svg) 0 0 0 0 stretch stretch;"
                                "}");
        checkBox->setAttribute(Qt::WA_TranslucentBackground, true);
    }

    QList<QPushButton*> buttons = findChildren<QPushButton*>();

    for(QPushButton* button: buttons){
        QFont font = button->font();
        font.setPointSize(12);
        font.setFamily("Sans Serif");
        button->setFont(font);

        button->setMaximumSize(500, 100);

        button->setStyleSheet("QPushButton { "
                              "border-image: url(:/backgrounds/backgrounds/shwButtonBackground.svg) 0 0 0 0 stretch stretch;"
                              "border-radius: 15px;"
                              "text-align: center;}"
                              "QPushButton:hover"
                              "{"
                              "border-image: url(:/backgrounds/backgrounds/hoverShwButtonBackground.svg) 0 0 0 0 stretch stretch;"
                              "}");
    }
}
