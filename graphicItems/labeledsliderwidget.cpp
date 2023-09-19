#include "labeledsliderwidget.h"
#include "ui_labeledsliderwidget.h"

#include <QGraphicsDropShadowEffect>

constexpr int kMax = 170, kMin = 25;

LabeledSliderWidget::LabeledSliderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LabeledSlider)
{
    ui->setupUi(this);

    value = 0;
    max = kMax;
    min = kMin;

    ui->horizontalSlider->setMaximum((max - min) / 5);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderChanged(int)));
    ui->label->setText(QString::number(value)+tr(" μs"));
}

LabeledSliderWidget::~LabeledSliderWidget()
{
    delete ui;
}

void LabeledSliderWidget::onSliderChanged(int newValue){

    value = newValue * 5 + min;

    ui->label->setText(QString::number(value)+tr(" μs"));
}

void LabeledSliderWidget::setValue(int newValue)
{
    if (newValue >= min && newValue <= max) {
        value = newValue;
    }
    else if (newValue < min) {
        value = min;
    }
    else {
        value = max;
    }

    ui->horizontalSlider->setValue((value - min) / 5);
    ui->label->setText(QString::number(value)+tr(" μs"));
}

void LabeledSliderWidget::setMin(int newMin)
{
    min = newMin;
    ui->horizontalSlider->setMaximum((max - min) / 5);
    setValue(value);
}

void LabeledSliderWidget::setMax(int newMax)
{
    max = newMax;
    ui->horizontalSlider->setMaximum((max - min) / 5);
    setValue(value);
}

int LabeledSliderWidget::getValue() const
{
    return value;
}
