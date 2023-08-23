#include "recievedialog.h"
#include "ui_recievedialog.h"

RecieveDialog::RecieveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecieveDialog)
{
    ui->setupUi(this);
    setFixedSize(400, 80);
}

RecieveDialog::~RecieveDialog()
{
    delete ui;
}

void RecieveDialog::setProgressValue(int value){
    ui->progressBar->setValue(value);
    if(value == 40){
        accept();
    }
}
