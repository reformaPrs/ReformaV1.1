#ifndef INTERVALDIALOG_H
#define INTERVALDIALOG_H

#include <QDialog>
#include <QVector>
#include <QListWidgetItem>
#include <QTime>

#include <dotsPairs/miosuit.h>

#include <bodyParts/body.h>

#include <dotsPairs/intervaldotspair.h>
#include "intervalsimulation.h"
#include "timeaxis.h"

namespace Ui {
class IntervalDialog;
}

/*!
 * \brief Класс диалогового окна интервальности
 *
 * Не используется в данной версии
 */
class IntervalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IntervalDialog(QWidget *parent = nullptr);
    ~IntervalDialog();

    /*!
     * \brief addPairs добавляет новые пары
     * \param pairs
     *
     * Добавляет новые пары в список и в шкалу, последняя пара становиться активной
     */
    void addPairs(int count);

    /*!
     * \brief translate переводит интерфейс
     */
    void translate();

    /*!
     * \brief Enum для выбора верха или низа тела
     */
    enum ActiveMode{
        UP,
        DOWN
    };
    Q_ENUM(ActiveMode);

    bool isUpActive();

    void updatePairs();

    void setIsAdminModeActive(bool newIsAdminModeActive);
signals:
    /*!
     * \brief add вызывает при нажатии на кнопку "Добавить"
     */
    void add();

    /*!
     * \brief partChanged вызывается при изменении верхней и нижней части тела
     */
    void partChanged(int);
private:
    Ui::IntervalDialog *ui;

    /*!
     * \brief changedByUser - изменено время начала или конца пользователем в поле времени или нет
     *
     * Нужно для избегания зацикливания при синхронизации редактирования времени с помощью бегунков и с помощью полей.
     */
    bool changedByUser = true;

    Body *body;
    MioSuit *mioSuit;

    /*!
     * \brief activeListWidget - активный список
     *
     * Либо список верхней части тела, либо нижней
     */
    QListWidget *activeListWidget = nullptr;

    /*!
     * \brief activeTimeAxis - активная временная ось
     *
     * Лмбо временная ось верхней части тела, либо нижней
     */
    TimeAxis *activeTimeAxis = nullptr;

    /*!
     * \brief activeDotsPair - активная пара контактов выбранная в списке
     */
    IntervalDotsPair *activeDotsPair = nullptr;

    /*!
     * \brief activeMode - активная часть тела (верхняя или нижняя)
     */
    ActiveMode activeMode = UP;

    bool isAdminModeActive = false;
private slots:
    /*!
     * \brief onAddClicked вызывается при нажатии кнопки "Добавить"
     *
     * В результате скрывает окно и ожидает добавления пар контактов, при этом вызывает add()
     */
    void onAddClicked();

    /*!
     * \brief onDeleteClicked вызывается при нажатии на кнопку "Удалить"
     *
     * В результате удаляет активную пару контактов и вызывает intervalPartDeleted(int)
     */
    void onDeleteClicked();

    /*!
     * \brief onSaveClicked вызывается при нажатии на кнопку "Сохранить"
     *
     * В результате скрывает окно
     */
    void onSaveClicked();

    /*!
     * \brief onShowClicked вызывается при нажатии на кнопку "Просмотр"
     *
     * Вызывает диалоговое окно симуляции
     */
    void onShowClicked();

    /*!
     * \brief onStartChanged вызывается при изменении времени начала интервала через шкалу
     * \param seconds
     */
    void onStartChanged(int seconds);

    /*!
     * \brief onEndChanged вызывается при изменении времени конца интервала через шкалу
     * \param seconds
     */
    void onEndChanged(int seconds);

    /*!
     * \brief onSpinBoxStartChanged вызывается при изменении времени начала интервала в SpinBox'е
     * \param seconds
     */
    void onSpinBoxStartChanged(int seconds);

    /*!
     * \brief onSpinBoxEndChanged вызывается при изменении времени конца интервала в SpinBox'е
     * \param seconds
     */
    void onSpinBoxEndChanged(int seconds);

    /*!
     * \brief onComboBoxIndexChanged вызывается при выборе части тела
     * \param index
     */
    void onComboBoxIndexChanged(int index);

    /*!
     * \brief onListItemClicked вызывается при нажатии на объект листа
     * \param item
     */
    void onListItemClicked(QListWidgetItem *item);
};

#endif // INTERVALDIALOG_H
