#include "licencetext.h"
#include "ui_licencetext.h"

LicenceText::LicenceText(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenceText)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onPrintButtonClicked()));
    QFont FontForPrint;
    FontForPrint.setPointSize(10);
    QFile file(":/data/licenceAgreement.txt");
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        QTextStream in(&file);
        ui->textEdit->setPlainText(in.readAll());
        ui->textEdit->setFont(FontForPrint);
        ui->textEdit->setReadOnly(true);
    }
}

LicenceText::~LicenceText()
{
    delete ui;
}

void LicenceText::onPrintButtonClicked()
{
   QPrinter printer(QPrinter::HighResolution);
   printer.setPageMargins(QMarginsF(10,10,10,10), QPageLayout::Millimeter);
      QPrintDialog *dlg = new QPrintDialog(&printer, this);
      dlg->setWindowTitle(tr("Print Document"));
      if (dlg->exec() == QDialog::Accepted)
      {
          ui->textEdit->print(&printer);
      }
      delete dlg;
}


