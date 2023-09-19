#ifndef PROGRAMWIDGET_H
#define PROGRAMWIDGET_H

#include "dotsPairs/programslidersvalues.h"
#include <QPushButton>
#include <QWidget>
#include <QPainter>

namespace Ui {
class ProgramWidget;
}

class ProgramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramWidget(QWidget *parent = nullptr);
    ~ProgramWidget();

    void setIsHalfhourActive(bool newIsHalfhourActive);

    bool getIsHalfhourActive() const;

    ProgramSlidersValues getProgramValues() const;

    int getHalfhourIndex();

    void setHalfHourIndex(int index);

    void clear();

    void setSlidersBoundries(ProgramSlidersValues minValues, ProgramSlidersValues maxValues);

    void setSlidersValues(const ProgramSlidersValues &values);

    void adminMode(bool isActive);

    void saveToFile(QDataStream *out);
    void readFromFile(QDataStream *in);
    static void createConfigData(QDataStream *out);
    void setProgramValuesForSave();

    void translate();
signals:
    void showClicked();
private:
    Ui::ProgramWidget *ui;
    QPushButton* firstHalfhourBtn;
    QPushButton* secondHalfhourBtn;
    QButtonGroup* halfhourBtnsGroup;
    ProgramSlidersValues programValues;
    ProgramSlidersValues slidersValues;
    ProgramSlidersValues slidersMinValues;
    ProgramSlidersValues slidersMaxValues;
    bool isCheckBoxChecked[2][9];
    bool isHalfhourActive = false;

    void setProgramValues(int index);
    void saveHalfHour(int index);
    void setHalfhour(int index);

    void setHalfhourSlidersBoundries(int index);

    void setHalfhourSlidersValue(int index);
private slots:
    void onShowClicked();
    void onCheckBoxStateChanged(int state);
    void onHalfhourToggled(QAbstractButton*,bool);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // PROGRAMWIDGET_H
