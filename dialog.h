#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QPropertyAnimation>
#include <QTimer>

#include <qtimer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void setLoadingValue(int value);
private slots:
    void startMainWindow();
private:
    Ui::Dialog *ui;

    QPropertyAnimation* progressBarAnimation;
    QTimer* progressBarTimer;
    int targetProgress;
};
#endif // DIALOG_H
