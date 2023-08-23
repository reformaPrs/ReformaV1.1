#include "jsonclient.h"
#include "ui_jsonclient.h"

const QString kLoginFileName = "login.save";

jsonclient::jsonclient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::jsonclient)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
    QFile file(kLoginFileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_13);
        QString login;
        in >> login;
        ui->lineEdit->setText(login);
        file.close();
    }
    ConnectDataBase();
    socket->waitForReadyRead(500);
}

jsonclient::~jsonclient()
{
    delete ui;
}

void jsonclient::ConnectDataBase()
{
   socket->connectToHost("81.177.159.23", 55555);
}
void jsonclient::sockDisc(){
    socket->deleteLater();
}
void jsonclient::sockReady(){
    if(socket->waitForConnected(500)){
        socket->waitForReadyRead(500);
        Data=socket->readAll();
        qDebug()<<Data;
        doc=QJsonDocument::fromJson(Data, &docError);
        if(docError.errorString().toInt()==QJsonParseError::NoError){
            if((doc.object().value("type").toString()=="connect")&&(doc.object().value("status").toString()=="yes")){
                QMessageBox::information(this,"Информация","Соединение установлено");
            }
            else if(doc.object().value("type").toString()=="resultCheck"){
                check = doc.object().value("status").toString();
            }
            else{
                QMessageBox::information(this,"Информация","Соединение не установлено");
            }
        }
        else{
            QMessageBox::information(this,"Информация","Ошибка с форматом передачи данных"+docError.errorString());
        }
    }
}


void jsonclient::on_InButton_clicked()
{
    QFile file(kLoginFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
    } else {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_13);
        QString login;
        out << ui->lineEdit->text();
        file.close();
    }
    QByteArray checkUser = "{\"type\":\"check\",\"login\":\"";
    checkUser.append(QString(ui->lineEdit->text()+"\"}").toUtf8());
    socket->write(checkUser);
    socket->waitForReadyRead(500);
    this->close();
}

void jsonclient::on_OutButton_clicked()
{
    this->close();
    check="close";
}
