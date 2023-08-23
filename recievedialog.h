#ifndef RECIEVEDIALOG_H
#define RECIEVEDIALOG_H

#include <QDialog>

namespace Ui {
class RecieveDialog;
}

class RecieveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecieveDialog(QWidget *parent = nullptr);
    ~RecieveDialog();

    void setProgressValue(int value);
private:
    Ui::RecieveDialog *ui;
};

#endif // RECIEVEDIALOG_H
