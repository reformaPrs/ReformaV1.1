#ifndef LABELEDSLIDERWIDGET_H
#define LABELEDSLIDERWIDGET_H

#include <QWidget>

namespace Ui {
class LabeledSlider;
}

class LabeledSliderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LabeledSliderWidget(QWidget *parent = nullptr);
    ~LabeledSliderWidget();

    int getValue() const;

    void setValue(int newValue);
    void setMin(int newMin);
    void setMax(int newMax);

private slots:
    void onSliderChanged(int newValue);
private:
    Ui::LabeledSlider *ui;
    int value;
    int max, min;
};

#endif // LABELEDSLIDERWIDGET_H
