#include "dialog.h"
#include "ui_dialog.h"

#include "mainwindow.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlag( Qt::FramelessWindowHint);

    QTimer::singleShot(500, this, SLOT(startMainWindow()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::startMainWindow(){
    qDebug() << "MainWindow start";
    MainWindow *m = new MainWindow(this);
    m->show();
}


void Dialog::setLoadingValue(int value){
    ui->progressBar->updateProgressInThread(value);
    if(value == 100)
        close();
}
