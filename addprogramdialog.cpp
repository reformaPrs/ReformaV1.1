#include "addprogramdialog.h"
#include "ui_addprogramdialog.h"

AddProgramDialog::AddProgramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProgramDialog)
{
    ui->setupUi(this);

    initProgramList();

    if(!programList.isEmpty()){
        ui->categoryComboBox->addItems(programList);
    }

    connect(ui->okPushButton, SIGNAL(clicked(bool)), this, SLOT(okPushButtonClicked()));
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

AddProgramDialog::~AddProgramDialog()
{
    delete ui;
}

void AddProgramDialog::okPushButtonClicked()
{
    QString category;
    switch (ui->categoryComboBox->currentIndex()) {
    case 0:
        category = "/cp/";
        break;
    case 1:
        category = "/tcp/";
        break;
    case 2:
        category = "/stroke/";
        break;
    case 3:
        category = "/svmd/";
        break;
    case 4:
        category = "/spinal/";
        break;
    case 5:
        category = "/neuro/";
        break;
    case 6:
        category = "/congenital/";
        break;
    default:
        category = "/other/";
        break;
    }

    if(ui->filenameLineEdit->text().trimmed().isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите имя программы.");
    }else{
        fullProgramName = category + ui->filenameLineEdit->text().trimmed();

        this->done(QDialog::Accepted);
    }
}

void AddProgramDialog::cancelPushButtonClicked()
{
    this->done(QDialog::Rejected);
    this->close();
}

void AddProgramDialog::initProgramList()
{
    QFile file(":/data/programsCategory.csv");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot open file";
        return;
    }

    QTextStream in(&file);
    while(!in.atEnd()){
        QString line = in.readLine();

        QStringList fields = line.split(',');

        if(fields.size() > 1){
            programList.append(fields.at(1));
        }
    }

    file.close();
}
