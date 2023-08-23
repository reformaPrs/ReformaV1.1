#ifndef ADDPROGRAMDIALOG_H
#define ADDPROGRAMDIALOG_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class AddProgramDialog;
}

class AddProgramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddProgramDialog(QWidget *parent = nullptr);
    ~AddProgramDialog();

    QString fullProgramName;

private slots:
    void okPushButtonClicked();
    void cancelPushButtonClicked();

private:
    Ui::AddProgramDialog *ui;

    QList<QString> programList;

    void initProgramList();
};

#endif // ADDPROGRAMDIALOG_H
