#include "licenceupdate.h"
#include "ui_licenceupdate.h"

LicenceUpdate::LicenceUpdate(QDate expireDate, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::LicenceUpdate)
{
    ui->setupUi(this);
    ui->retranslateUi(this);
    if(expireDate > QDate::currentDate()){
        int days = expireDate.dayOfYear() - QDate::currentDate().dayOfYear();
        ui->updateLabel->setText(tr("Your program will need to be updated in ") + QString::number(days) +
                                 tr(" days. ") + tr("Update your program before ") + expireDate.toString()+
                                 ".\n" + tr("License renewal is possible after recertification. All questions you can ask at SMART Academy academsmart@yandex.ru"));
    } else {
        ui->updateLabel->setText(tr("Your program needs to be updated") +
                                 ".\n" + tr("License renewal is possible after recertification. All questions you can ask at SMART Academy academsmart@yandex.ru"));
    }
}

LicenceUpdate::~LicenceUpdate()
{
    delete ui;
}
