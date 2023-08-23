#ifndef CHOOSINGDIALOG_H
#define CHOOSINGDIALOG_H

#include <QDialog>
#include <QList>
#include <QListWidgetItem>
#include <QMessageBox>

namespace Ui {
class ChoosingDialog;
}

class QListWidgetItem;

class ChoosingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoosingDialog(QWidget *parent = nullptr);
    ~ChoosingDialog();

    void initList(const QStringList &list);
    int currentIndex() const;
    int count() const;
    QString currentText() const;
    bool checkProgramNameExist(const QString &programName) const;
    void addItem(const QString &text);
    void setCurrentIndex(int index);
    void removeItem(int index);
    void initFilteredListBySubstring(const QStringList &list, const QString &substring);
    QString cutString(const QString &string);

    void translate();
signals:
    void indexChanged(int);
private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
private:
    Ui::ChoosingDialog *ui;

    QString currentChoosingProgramState;
    QList<int> currentProgramIndexs;
};

#endif // CHOOSINGDIALOG_H
