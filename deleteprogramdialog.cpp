#include "deleteprogramdialog.h"
#include "ui_deleteprogramdialog.h"

DeleteProgramDialog::DeleteProgramDialog(const QStringList &programList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteProgramDialog)
{
    ui->setupUi(this);

    if(!programList.isEmpty()){
        foreach (QString item, programList) {
            ui->programComboBox->addItem(cutString(item));
        }
    }

    connect(ui->okDeletePushButton, SIGNAL(clicked(bool)), this, SLOT(okDeletePushButtonClicked()));
    connect(ui->cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(cancelPushButtonClicked()));

    this->setStyleSheet("QPushButton"
                        "{"
                        "border: 1px;"
                        "border-color: gray;"
                        "}"
                        "QPushButton:hover"
                        "{"
                        "background-color: #dcdcde;"
                        "}");
}

DeleteProgramDialog::~DeleteProgramDialog()
{
    delete ui;
}

void DeleteProgramDialog::okDeletePushButtonClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":icons/icons/reforma1.ico"));
    msgBox.setText(tr("Do you want to delete ") + ui->programComboBox->currentText() + tr(" program?"));
    msgBox.setIcon(QMessageBox::Question);
    QPushButton *yesBtn = msgBox.addButton(tr("Yes"), QMessageBox::ActionRole);
    QPushButton *noBtn = msgBox.addButton(tr("No"), QMessageBox::ActionRole);
    msgBox.exec();

    if(msgBox.clickedButton() == (QAbstractButton *) yesBtn){
        qDebug() << "yes";
        choosedProgramName = ui->programComboBox->currentText();
        msgBox.done(QDialog::Accepted);
        this->done(QDialog::Accepted);
    }else if(msgBox.clickedButton() == (QAbstractButton *) noBtn){
        qDebug() << "no";
        msgBox.close();
    }
}

void DeleteProgramDialog::cancelPushButtonClicked()
{
    this->done(QDialog::Rejected);
    this->close();
}

QString DeleteProgramDialog::cutString(const QString string)
{
    int startIndex = string.indexOf('/');
    int endIndex = string.lastIndexOf('/');

    if(startIndex != -1 && endIndex != -1 && endIndex > startIndex){
        QString substringBetweenSlashes = string.mid(startIndex + 1, endIndex - startIndex - 1);

        QString newString = string;

        return newString.remove("/" + substringBetweenSlashes + "/");
    }
    return NULL;
}
