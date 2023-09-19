#include "customsliderwidget.h"
#include "ui_customsliderwidget.h"

#include <qpainter.h>



CustomSliderWidget::CustomSliderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomSliderWidget)
{
    ui->setupUi(this);

    value = 0;

    setAttribute(Qt::WA_TranslucentBackground);

    const int max = 170, min =20;

    QFont font = ui->label->font();
    font.setPointSize(11);
    ui->label->setFont(font);

    ui->horizontalSlider->setMaximum((max - min) / 5);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderChanged(int)));
}

CustomSliderWidget::~CustomSliderWidget()
{
    delete ui;
}

void CustomSliderWidget::onSliderChanged(int newValue)
{
    if (newValue !=0)
        value = newValue*5+20;
    else
        value = newValue;

    ui->label->setText(QString::number(value)+tr(" μs"));
    emit impulseClicked(value);
}

void CustomSliderWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked();
}

void CustomSliderWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QWidget::paintEvent(event);
}

void CustomSliderWidget::retranslateUI(){
    ui->label->setText(QString::number(value)+tr(" μs"));
}

void CustomSliderWidget::setValue(int newValue)
{
    value = newValue;
    ui->horizontalSlider->setValue((newValue - 20) / 5);
}
