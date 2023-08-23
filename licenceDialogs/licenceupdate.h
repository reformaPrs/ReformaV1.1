#ifndef LICENCEUPDATE_H
#define LICENCEUPDATE_H

#include <QDate>
#include <QDialog>

namespace Ui {
class LicenceUpdate;
}

/*!
 * \brief Класс диалогового окна с предупреждением об окончании лицензии
 */
class LicenceUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit LicenceUpdate(QDate expireDate, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~LicenceUpdate();

private:
    Ui::LicenceUpdate *ui;
};

#endif // LICENCEUPDATE_H
