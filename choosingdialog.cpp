#include "choosingdialog.h"
#include "ui_choosingdialog.h"

ChoosingDialog::ChoosingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoosingDialog)
{
    ui->setupUi(this);
    currentProgramIndexs = *new QList<int>();


}

ChoosingDialog::~ChoosingDialog()
{
    delete ui;
}

void ChoosingDialog::initList(const QStringList &list)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(list);
}

int ChoosingDialog::currentIndex() const
{
    if(currentProgramIndexs.empty())
        return ui->listWidget->currentRow();
    return currentProgramIndexs.at(ui->listWidget->currentRow());
}

int ChoosingDialog::count() const
{
    return ui->listWidget->count();
}

QString ChoosingDialog::currentText() const
{
    return "/" + currentChoosingProgramState + "/" + ui->listWidget->currentItem()->text();
}

bool ChoosingDialog::checkProgramNameExist(const QString &programName) const
{
    for (int i = 0; i < ui->listWidget->count(); i++){
        if (programName == ui->listWidget->item(i)->text()){
            return true;
        }
    }
    return false;
}

void ChoosingDialog::addItem(const QString &text)
{
    ui->listWidget->addItem(text);
}

void ChoosingDialog::setCurrentIndex(int index)
{
    ui->listWidget->setCurrentRow(index);
    emit indexChanged(index);
}

void ChoosingDialog::removeItem(int index)
{
    ui->listWidget->removeItemWidget(ui->listWidget->item(index));
}

void ChoosingDialog::initFilteredListBySubstring(const QStringList &list, const QString &substring)
{
    ui->listWidget->clear();
    currentChoosingProgramState = substring;
    currentProgramIndexs.clear();

    for(const QString& itemText : list){
        int startIndex = itemText.indexOf('/');
        int endIndex = itemText.lastIndexOf('/');

        if(startIndex != -1 && endIndex != -1 && endIndex > startIndex){
            QString substringBetweenSlashes = itemText.mid(startIndex + 1, endIndex - startIndex - 1);

            if(substringBetweenSlashes == substring){
                currentProgramIndexs.append(list.indexOf(itemText));
                QString updateItemText = itemText;
                updateItemText.remove("/" + substringBetweenSlashes + "/");
                ui->listWidget->addItem(updateItemText);
            }
        }
    }
}

QString ChoosingDialog::cutString(const QString &string)
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


void ChoosingDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int index;
    if(currentProgramIndexs.empty())
    {
        index = ui->listWidget->indexFromItem(item).row();
    }
    else{
        index = currentProgramIndexs.at(ui->listWidget->indexFromItem(item).row());
    }

    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/icons/icons/reforma1.ico"));
    msgBox.setText(tr("You chose ") + item->text());
    msgBox.setIcon(QMessageBox::Question);
    QPushButton *yesBtn = msgBox.addButton(tr("Continue"), QMessageBox::ActionRole);
    QPushButton *noBtn = msgBox.addButton(tr("Cancel"), QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == (QAbstractButton *) yesBtn) {
        qDebug() << "yes";
        emit indexChanged(index);
    } else if (msgBox.clickedButton() == (QAbstractButton *) noBtn) {
        qDebug() << "no";
        return;
    }
}

void ChoosingDialog::translate()
{
    ui->retranslateUi(this);
}

