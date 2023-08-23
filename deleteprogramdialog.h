#ifndef DELETEPROGRAMDIALOG_H
#define DELETEPROGRAMDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class DeleteProgramDialog;
}

class DeleteProgramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteProgramDialog(const QStringList &programList, QWidget *parent = nullptr);
    ~DeleteProgramDialog();

    QString choosedProgramName;
private slots:
    void okDeletePushButtonClicked();
    void cancelPushButtonClicked();
private:
    Ui::DeleteProgramDialog *ui;

    QString cutString(const QString string);
};

#endif // DELETEPROGRAMDIALOG_H
