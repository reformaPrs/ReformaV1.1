#ifndef TABSWIDGET_H
#define TABSWIDGET_H

#include "choosingdialog.h"
#include "programwidget.h"
#include <MainWindow.h>
#include <QVBoxLayout>
#include <QWidget>

#include <QDir>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QPalette>

namespace Ui {
class TabsWidget;
}

class TabsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TabsWidget(QWidget *parent = nullptr);
    ~TabsWidget();

    /*!
     * \brief getProgramWidget возвращает виджет с отображением текущей программы
     */
    ProgramWidget *getProgramWidget() const;

    /*!
     * \brief getPanelWidget возвращает виджет с списком категорий программ
     */
    QWidget *getPanelWidget() const;

    /*!
     * \brief getLabel возвращает надпись для отображения названия программы
     */
    QLabel *getLabel() const;

    /*!
     * \brief getCurrentProgramIndex возвращает индекс текущей программы относительно общего списка программ
     */
    int getCurrentProgramIndex() const;

    /*!
     * \brief getNumberOfPrograms возвращает общее количество программ
     */
    int getNumberOfPrograms() const;

    /*!
     * \brief activateAdminMode активирует режим администратора
     *
     * Активирует режим администратора для programWidget
     */
    void activateAdminMode();

    /*!
     * \brief deactivateAdminMode деактивирует режим администратора
     *
     * Деактивирует режим администратора для programWidget
     */
    void deactivateAdminMode();

    /*!
     * \brief getCurrentProgramName возвращает имя текущей выбранной программы
     */
    const::QString getCurrentProgramName() const;

    /*!
     * \brief readProgramFromFile считывает программу из потока для programWidget
     *
     * \param in - поток из файла
     * \param programName - имя программы
     */
    bool readProgramFromFile(QDataStream *in, const QString &programName);

    /*!
     * \brief checkProgramNameExist проверяет существует ли программа с таким именем
     *
     * \param programName - имя искомой программы
     */
    bool checkProgramNameExist(const QString& programName) const;

    /*!
     * \brief initProgramList инициализирует список для выбранной категории
     */
    void initProgramList();

    /*!
     * \brief addProgramName добавляет новое имя в общий список программ
     */
    bool addProgramName(const QString& programName);

    /*!
     * \brief translate используется для перевода текста, запускает команду ui->retranslate();
     */
    void translate();

    /*!
     * \brief setProgramNames устаналивает получаемый список имен программ
     *
     * \param programNames - общий список имен программ
     */
    void setProgramNames(const QStringList &programNames);
private slots:
    /*!
     * \brief onAddPushButton_clicked вызывается при нажатии на кнопку "Добавить программу"
     *
     * Вызывает диалоговое окно для добавления новой программы
     */
    void onAddPushButton_clicked();

    /*!
     * \brief onDeletePushButton_clicked вызывается при нажатии на кнопку "Удалить выбранную программу"
     *
     * Вызывает диалоговое окно для удаления выбранной программы
     */
    void onDeletePushButton_clicked();

    /*!
     * \brief onProgramShowClicked вызывается при нажатии на кнопку "Просмотр"
     *
     * Издает сигнал об отображении текущей программы на теле
     */
    void onProgramShowClicked();

    /*!
     * \brief onCPPushButton_clicked вызывается при нажатии на кнопку списка программ с категорией "ДЦП"
     */
    void onCPPushButton_clicked();

    /*!
     * \brief onTCPPushButton_clicked вызывается при нажатии на кнопку списка программ с категорией "ЧМТ"
     */
    void onTCPPushButton_clicked();

    /*!
     * \brief onStrokePushButton_clicked вызывается при нажатии на кнопку списка программ с категорией "Инсульт"
     */
    void onStrokePushButton_clicked();

    /*!
     * \brief onSVMDPushButton_clicked вызывается при нажатии на кнопку списка программ с категорией "СВМП"
     */
    void onSVMDPushButton_clicked();

    /*!
     * \brief onCongenitalDisordersPushButton_clicked вызывается при нажатии на кнопку списка программ с категорией "Врожденные"
     */
    void onCongenitalDisordersPushButton_clicked();

    /*!
     * \brief onNeuromuscularMuscularDisordersPushButton_clicked вызывается при нажатии на кнопку списка программ с категорией "Нейро-мышечные"
     */
    void onNeuromuscularMuscularDisordersPushButton_clicked();

    /*!
     * \brief onSpinalDisordersPushButton_clicked вызывается при нажатии на кнопку списка программ с категорией "Спинальные"
     */
    void onSpinalDisordersPushButton_clicked();

    /*!
     * \brief onOtherPushButtton_clicked вызывается при нажатии на кнопку списка программ с категорией "Другое"
     */
    void onOtherPushButtton_clicked();

    /*!
     * \brief onProgramChanged вызывается при выборе программы для отображения ее настроек в programWidget
     */
    void onProgramChanged(int index);
signals:
    /*!
     * \brief showProgram сигнал для отображения программы на модели человека
     */
    void showProgram(int);

    /*!
     * \brief programChanged сигнал для отображения и/или смены программы
     */
    void programChanged(int);

    /*!
     * \brief programAdded сигнал после добавления новой программы в режиме администратора
     *
     * \param int item1 - индекс новой программы
     * \param QString item2 - имя новой программы
     */
    void programAdded(int, QString);

    /*!
     * \brief programDeleted сигнал после удаления программы в режиме администратора
     *
     * \param int item1 - индекс удаленной программы
     * \param QString item2 - имя удаленной программы
     */
    void programDeleted(int, QString);
private:
    Ui::TabsWidget *ui;

    bool isAdminMode = false; ///< текущее состояние режима администратора

    QPushButton* cpPushButton; ///< кнопка категории "ДЦП"
    QPushButton* tcpPushButton; ///< кнопка категории "ЧМТ"
    QPushButton* strokePushButton; ///< кнопка категории "Инсульт"
    QPushButton* svmdPushButton; ///< кнопка категории "СВМП"
    QPushButton* congenitalDisordersPushButton; ///< кнопка категории "Врожденные"
    QPushButton* neuroMuscularPushButton; ///< кнопка категории "Нейро-мышечные"
    QPushButton* spinalDisordersPushButton; ///< кнопка категории "Спинальные"
    QPushButton* otherPushButton; ///< кнопка категории "Другое"

    ChoosingDialog *programChoosingDialog; ///< диалоговое окно с списком программ выбранной категории
    QStringList programNames; ///< список всех программ по именам

    /*!
     * \brief initButtons инициализирует кнопки для tabsWidget
     */
    void initButtons();

    /*!
     * \brief openChoosingDialog открывает диалоговое окно с выбором программ по выбранной категории
     *
     * \param QString substring - подстрока для поиска программ из общего списка
     */
    void openChoosingDialog(const QString& substring);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // TABSWIDGET_H
