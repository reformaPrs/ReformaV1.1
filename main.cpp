#include "config.h"
#include "dialog.h"

#include <QApplication>
#include <QElapsedTimer>
#include <QPainter>
#include <QSplashScreen>
#include "jsonclient.h"

QString checkLincenc(){
    jsonclient* client = new jsonclient();
    client->setWindowFlags(Qt::FramelessWindowHint);
    client->exec();
    while (client->check==""){
        QMessageBox::information(client,"Информация","Вы ввели неверный логин");
        client->exec();
    }
    return client->check;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString check = "";
    QDialog d;

    if (kIsProfVersion){
        check = checkLincenc();
    } else {
        check = "Имеет доступ";
    }
    if (check == "Не имеет доступ") {
        QMessageBox::information(&d, "Информация","У вас закончилась лицензия!");
        return 0;
    }
    if (check == "Заблокирован") {
        QMessageBox::information(&d, "Информация","У вас ограничины возможности использования!");
        return 0;
    }
    if (check == "" || check == "close"){
        return 0;
    }

//    if(kEnableDateExpiration){
//        QDate expireDate = QDate(EXPIRATION_DATE);
//        QDate warningDate = QDate::currentDate().addMonths(1);

//        if(expireDate < warningDate || expireDate < QDate::currentDate()){
//            LicenceUpdate l(expireDate);
//            l.exec();
//            if(expireDate < QDate::currentDate())
//                return 0;
//        }
//    }
    Dialog loadingDialog;
    loadingDialog.show();
    return a.exec();;
}
