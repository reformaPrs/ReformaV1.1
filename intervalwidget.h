#ifndef INTERVALWIDGET_H
#define INTERVALWIDGET_H

#include <QListWidget>
#include <QWidget>
#include <QButtonGroup>

#include <timeaxis.h>
#include <bodyParts/body.h>
#include <dotsPairs/miosuit.h>

namespace Ui {
class IntervalWidget;
}

class IntervalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IntervalWidget(QWidget *parent = nullptr);
    ~IntervalWidget();

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
    /*!
     * \brief updatePairs обновляет пары виджета
     */
    void updatePairs();


signals:
    /*!
     * \brief partChanged вызывается при изменении верхней и нижней части тела
     */
    void partChanged(int);
    /*!
     * \brief showClicked вызывается при нажатии на кнопку Просмотр
     */
    void showClicked();

private:
    Ui::IntervalWidget *ui;
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

    QButtonGroup *selectionButtonGroup;

private slots:
    /*!
     * \brief onDeleteClicked вызывается при нажатии на кнопку "Удалить"
     *
     * В результате удаляет активную пару контактов и вызывает intervalPartDeleted(int)
     */
    void onDeleteClicked();
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
     * \brief onListItemClicked вызывается при нажатии на объект листа
     * \param item
     */
    void onListItemClicked(QListWidgetItem *item);
    /*!
     * \brief onSelectionButtonToggled вызывается при смене режима верха/низа тела кнопками
     */
    void onSelectionButtonToggled(QAbstractButton *, bool);

    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
};

#endif // INTERVALWIDGET_H
