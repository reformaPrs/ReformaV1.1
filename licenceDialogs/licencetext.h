#ifndef LICENCETEXT_H
#define LICENCETEXT_H

#include <QDialog>
#include <QFile>
#include <QPrinter>
#include <QPrintDialog>
#include <QTimer>
#include <QDebug>
#include <QTextStream>

namespace Ui {
class LicenceText;
}

/*!
 * \brief Класс диалогового окна с текстом лицензии
 */
class LicenceText : public QDialog
{
    Q_OBJECT

public:
    explicit LicenceText(QWidget *parent = nullptr);
    ~LicenceText();

private slots:
    /*!
     * \brief onPrintButtonClicked печатает лицензию
     *
     * Вызывает окно для печати лицензии
     */
    void onPrintButtonClicked();
private:
    Ui::LicenceText *ui;
};

#endif // LICENCETEXT_H
