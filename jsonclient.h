#ifndef JSONCLIENT_H
#define JSONCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardItem>
#include <QDialog>
#include <QApplication>
#include <QFile>

namespace Ui {
class jsonclient;
}

class jsonclient : public QDialog
{
    Q_OBJECT

public:
    explicit jsonclient(QWidget *parent = nullptr);
    ~jsonclient();
    QTcpSocket* socket;
    QByteArray Data;

    QJsonDocument doc;
    QJsonParseError docError;
    QString check;
private:
    Ui::jsonclient *ui;
public slots:
    void sockReady();
    void sockDisc();
private slots:
    void ConnectDataBase();
    void on_InButton_clicked();
    void on_OutButton_clicked();
};

#endif // JSONCLIENT_H
