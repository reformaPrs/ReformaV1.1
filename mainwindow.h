#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>

#include <QPushButton>
#include <QActionGroup>
#include <QButtonGroup>
#include <QVBoxLayout>

#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsTextItem>

#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>

#include <QDateTime>
#include <QTimer>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTranslator>
#include <QShortcut>
#include <QToolButton>

#include <QStandardPaths>
#include <QInputDialog>
#include <QPushButton>

#include <bodyParts/body.h>
#include <dotsPairs/miosuit.h>
#include <ui_tabswidget.h>

#include "config.h"
#include "decoder.h"
#include "dialog.h"
#include "impulseadder.h"
#include "intervaldialog.h"
#include "recievedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

/*!
 * \brief Класс главного окна
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static const QString programsDir;

    MainWindow(Dialog *loadingDialog, QWidget *parent = nullptr);
    ~MainWindow();

    /*!
     * \brief The Packet enum - номера пакетов для общения с прибором
     */
    enum Packet{
        AH_PACKET_DATA_0 = 0x20, ///< Номер пакета с парами с правой стороны рубашки
        AH_PACKET_DATA_1 = 0x21, ///< Номер пакета с парами с левой стороны рубашки
        AH_PACKET_DATA_2 = 0x22, ///< Номер пакета с парами с правой стороны штанов
        AH_PACKET_DATA_3 = 0x23, ///< Номер пакета с парами с левой стороны штанов
        AH_PACKET_DATA_4 = 0x24, ///< Номер пакета с состоянием
        AH_COMMAND_PACKET = 0x25 ///< Номер пакета с командами от компьтера
    };
    Q_ENUM(Packet)

    /*!
     * \brief The Command enum - коды команд в пакете AH_COMMAND_PACKET
     */
    enum Command{
        STATUS_TICK = 0xA5, ///< Команда для проверки подключения
        GET_STATUS = 0x84, ///< Команда отправки информации о состоянии пар
        STATUS_HEATER = 0x9A, ///< Команда переключение нагревателя
        STATUS_START = 0x91, ///< Команда начала процедуры
        STATUS_STOP = 0x96, ///< Команда остановки процедуды
        STATUS_PAUSE = 0x99 ///< Команда паузы процедуры
    };
    Q_ENUM(Command)
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene; ///< Главная сцена с человеком
    QGraphicsTextItem *serialNumberText; ///< Текст с серийным номером прибора
    QGraphicsTextItem *fileNameText;
    QTimer *statusTickTimer; ///< Таймер для отправки пакета со статусом для поддержки подключения прибора
    QTimer *deviceConnectionTimer; ///< Таймер проверки подключения прибора
    bool isOldVersionProtocol = false; ///< True если в пакетах первая буква A

    QList<QAction *> portActions; ///< Вектор действий для выбора порта прибора
    QActionGroup *portsActionGroup; ///< Группа действий для выбора порта прибора
    QMenu *portsMenu; ///< Меню с действиями для выбора порта прибора
    QSerialPort port; ///< Активный порт прибора
    QSerialPortInfo portInfo; ///< Иинформация об активном порте
    QList<QSerialPortInfo> portsInfoList; ///< Вектор всех информаций о портах

    QPushButton *firstHalfhourButton; ///< Кнопка первого получаса
    QPushButton *secondHalfhourButton; ///< Кнопка первого получаса
    QPushButton *intervalPushButton; ///< Кнопка для отображения окна с интервалами
    QButtonGroup *halfHourButtons; ///< Группа кнопок для получасового задания

    QActionGroup *langGroup; ///< Группа действий для выбора языка
    QAction *actionEN; ///< Действие английского языка
    QAction *actionRU; ///< Действие русского языка
    QTranslator translatorRU; ///< Переводчик на русский язык
    QTranslator translatorEN; ///< Переводчик на английский язык
    const QString localSaveFileName = "/trInfo.save"; ///< Имя файла для сохранения установленного языка
    Language language; ///< Текущий язык интерфейса

    Body *body;
    MioSuit *mioSuit;

    ImpulseAdder *impulseAdder = nullptr; ///< Объект отображающий всплывающие мышцы для задания ипмульсов

    QShortcut *copyShortcut; ///< Комбинация клавиш для копирования
    QShortcut *clearShortcut;

    char commandPacket[6]; ///< Пакет для отправки команды на прибор
    char outDataPacket[17]; ///< Пакет для отправки данныхх о контактах на прибор
    QByteArray inputBuffer; ///< Буфер для получения пакетов от прибора
    RecieveDialog *recieveDialog; ///< Диалоговое окно получения данных с блока
    int recievedValues[40][40]; ///< Массив принятых с прибора значений, первый индекс номер пары, второй секунда интервала
    int intervalIndex = 0; ///< Индекс интервала в recievedValues
    unsigned long serialNumber; ///< Серийный номер подключенного устройстваж

    bool isIntervalActive = false; ///< Активно ли добаление в интервалы
    IntervalDialog *intervalDialog; ///< Виджет интервальности

    bool isAdminModeActive = false; ///< Состояние режима администратора

    QString appDataPath; ///< Путь хранения данных(файлов) с программами (абсолютный путь к папке + /programs/)
    bool isIntervalModeActive = false;

    /*!
     * \brief initButtons инициализирует кнопки интерфейса
     */
    void initButtons();

    /*!
     * \brief initGraphicsView инициализирует графические элементы интерфейса
     */
    void initGraphicsView();

    /*!
     * \brief initMenuBar инициализирует действия в верхней панеле
     */
    void initMenuBar();

    /*!
     * \brief initPortsMenu инициализирует меню портов
     */
    void initPortsMenu();

    /*!
     * \brief updatePortsList обновляет список портов
     */
    void updatePortsList();

    /*!
     * \brief initTranslators инициализирует переволчики
     */
    void initTranslators();

    /*!
     * \brief saveCurrentLocal сохраняет текущий язык интерфейса в файл
     * \param local
     */
    void saveCurrentLocal(QString local);

    /*!
     * \brief updateText обновляет текст
     *
     * Используется для перевода мест, которые не переводятся командой ui->retranslate();
     */
    void updateText();

    /*!
     * \brief sendCommand отправляет пакет с командой
     * \param command - команда
     */
    void sendCommand(Command command);

    /*!
     * \brief getCheckSum считает контрольную сумму пакета
     * \param data - массив данных
     * \param len - длина пакета
     * \return значение контрольной суммы
     */
    char getCheckSum(const char *data, int len);

    /*!
     * \brief sendDataPackets отсылает 4 пакета с данными о всех парах контактов
     * \param dotsPairs - вектор с парами контактов для отправки
     */
    void sendDataPackets(QVector<SimpleDotsPair *> dotsPairs);

    /*!
     * \brief sendTimingPacket отсылает пакет для интервалов
     * \param time - время интервала
     * \param index - номер пакетов
     * \param halfhour - номер получаса 0 - первый, 1 второй
     *
     * Для старой версии time - минуты процедуры, halfhour - часы процедуры, index - любое
     */
    void sendTimingPacket(int time, int halfhour, int index);

    /*!
     * \brief initIntervalDialog инициализирует диалоговое окно интервалов
     * \param dialog
     */
    void initIntervalWidget();

    /*!
     * \brief sendHalfhourData отправляет данные активного получаса
     * \param oldVersion - true - старая версия прошивки контроллера, false - новая
     */
    void sendHalfhourData(bool isOldVersion);

    /*!
     * \brief setRecievedValues обновляет пары в соответствии с полученными с прибора данными
     */
    void setRecievedValues();

    /*!
     * \brief loadDataFromFile используется для загрузки данных из файла
     * \param file - ссылка на объект файла, который нужно прочитать
     */
    bool loadDataFromFile(QFile &file);

    /*!
     * \brief loadDataFromConfigFile используется для загрузки данных из конфигурационного файла через QDataStream
     * \param in - предоставляет доступ к данным, считанным из конфигурационного файла
     */
    void loadDataFromConfigFile(QDataStream *in);

    /*!
     * \brief saveCurrentProgram используется для сохранения текущей программы в файл
     */
    void saveCurrentProgram();

    /*!
     * \brief updateProgram выполняет обновление программы или данных внутри приложения
     */
    void updateProgram();

    /*!
     * \brief initAppDataDir выполняет инициализацию директории данных приложения
     * \param customPath - переданный пользовательский путь. Если пустой, устанавливается путь по умолчанию
     */
    void initAppDataDir(const QString& customPath);

    /*!
     * \brief copyDataFromResources копирует файлы программ (с расширением .mioconfig) из ресурсов приложения в директорию по адресу appDataPath
     */
    void copyDataFromResources();

    /*!
     * \brief toDefaultSettings используется для восстановления настроек приложения до значений по умолчанию
     */
    void toDefaultSettings();

    /*!
     * \brief createProgramFileName используется для создания и форматирования имен файлов программ в приложении
     * \param index - номер файла(программы)
     */
    QString createProgramFileName(int index);

    /*!
     * \brief createProgramFileName метод обновляет содержимое файла по заданному имени, записывая в файл данные data
     * \param filename - имя файла включащее полный путь
     * \param data - данные в строковом формате для записи в файл
     */
    void updateFile(const QString& filename, const QString& data);

    /*!
     * \brief readFiles считывает файлы из appDataPath для сохранения имен программ
     */
    void readFiles();
private slots:
    /*!
     * \brief onOpenClicked вызывается нажатием на кнопку или действие "Открыть"
     *
     * Считывает из выбранного файла .mio данные для процедуры
     */
    void onOpenClicked();

    /*!
     * \brief onSaveClicked вызывается нажатием на кнопку или действие "Сохранить"
     *
     * Сохраняет в файл .mio данные для процедуры
     */
    void onSaveClicked();

    /*!
     * \brief onToolButtonHeatToggled вызывается нажатием на кнопку "Нагрев"
     * \param checked
     *
     * Переключает на выбор нагревателей и обратно на мышцы при повтоном нажатии
     */
    void onToolButtonHeatToggled(bool checked);

    /*!
     * \brief onSendClicked вызывается нажатием на кнопку или действие "Отправить"
     *
     * Отправляет данные о процедуре на прибор
     */
    void onSendClicked();

    /*!
     * \brief onRecieveClicked вызывается нажатием на кнопку или действие "Получить"
     *
     * получает данные о процедуре с прибор
     */
    void onRecieveClicked();

    /*!
     * \brief onPrintClicked вызывается нажатием на кнопку или действие "Печать"
     *
     * Печатает главное окно
     */
    void onPrintClicked();

    /*!
     * \brief onExportClicked вызывается нажатием на кнопку или действие "Экспорт"
     *
     * Экспортирует главное окно в pdf
     */
    void onExportClicked();

    /*!
     * \brief onExitClicked вызывается нажатием на кнопку или действие "Выход"
     *
     * Закрывает программу
     */
    void onExitClicked();

    /*!
     * \brief onIntervalWindowClicked вызывается при нажатии кнопки "Интервал"
     * Открывает режим интервалов
     */
    void onIntervalWindowClicked();

    /*!
     * \brief onIntervalWindowClicked вызывается при нажатии кнопки "Интервалы"
     * Открывает окно IntervalDialog
     */
    void onIntervalClicked();

    /*!
     * \brief onIntervalAddClicked вызывается при нажатии Добавить в окне интервальности
     */
    void onIntervalAddClicked();

    /*!
     * \brief onHalfhourToggled вызывается нажатием на одну из кнопок получаса
     * \param checked
     * Переключает с одного получаса на другой
     */
    void onHalfhourToggled(QAbstractButton *, bool checked);

    /*!
     * \brief onLicenceClicked вызывается нажатием на действие "Лицензия"
     *
     * Выводит окно с текстом лицензии LicanceText
     */
    void onLicenceClicked();


    /*!
     * \brief onProgramWidgetViewClicked вызывается нажатием на кнопку "Вернуться к настройке"
     *
     * На экране с отображением тела возвращает на экран программирования
     */
    void onProgramWidgetViewClicked();

    /*!
     * \brief onCopyShortcut вызывается нажатим на сочитание клавиш для копирования в системе
     *
     * Предлагает скопировать текущий получас в другой
     */
    void onCopyShortcut();

    /*!
     * \brief onProgramListClicked вызывается нажатим на кнопку или действие "Программы"
     *
     * Открывает главный экран с списком категорий программ
     */
    void onProgramListClicked();

    /*!
     * \brief onDeviceDisconnected вызывается при отключении прибора
     */
    void onDeviceDisconnected();

    /*!
     * \brief onPortTriggered вызывается при выборе порта прибора
     */
    void onPortTriggered();

    /*!
     * \brief onDeviceTriggered вызывается при нажатии на вкладку "Устройство"
     *
     * Вызывает updatePortsList() для обновления списка доступнх портов
     */
    void onDeviceTriggered();

    /*!
     * \brief sendStatusTick вызывается при срабатывании таймера statusTickTimer
     *
     * Вызывает sendCommand(Command command) для высылания пакета
     */
    void sendStatusTick();

    /*!
     * \brief onReadyRead вызывается при доступной информации для считывания с порта
     */
    void onReadyRead();

    /*!
     * \brief onBodyPartClicked вызывается при нажатии на часть тела
     * \param id - номер части тела
     *
     * В результате всплывает окно ImpulseAdder
     */
    void onBodyPartClicked(int partID);

    /*!
     * \brief onBackgroundClicked вызывается при нажатии на фон
     */
    void onBackgroundClicked();

    /*!
     * \brief onLanguageTriggered вызывается при выборе языка
     *
     * В результате изменяет язык интерфейса
     */
    void onLanguageTriggered();

    /*!
     * \brief onIntervalIntaractivePartChanged вызывается при изменении мышц в интервалах
     * \param index
     */
    void onIntervalIntaractivePartChanged(int index);

    /*!
     * \brief onImpulseAdderDone вызывает при закрытии всплывающих мышц для задания импульсов
     * \param result - результат изменения имупльсов
     */
    void onImpulseAdderDone(int result);

    /*!
     * \brief onHalfhourModeEnabled вызывается при включении режима получасов
     */
    void onHalfhourModeEnabled();

    /*!
     * \brief onHalfhourModeDisabled вызывается при отключении режима получасов
     */
    void onHalfhourModeDisabled();

    /*!
     * \brief onClearShortcut вызывается при Ctrl+N для очистки часа или получаса в режиме получасов
     */
    void onClearShortcut();

    /*!
     * \brief onProgramShow вызывается сигналом showProgram для отображения выбранной программы
     */
    void onProgramShow();

    /*!
     * \brief onProgramChanged вызывается сигналом programChanged для загрузки выбранной программы из файла
     * \param index - индекс программы для загрузки из файла
     */
    void onProgramChanged(int);

    /*!
     * \brief onAdminModeChanged вызывается при нажатии на действие "Администратор" для запроса кода администратора и активации/деактивации режима администратора
     */
    void onAdminModeChanged();

    /*!
     * \brief onProgramAdded вызывается сигналом TabsListWidget::programAdded при добавлении новой программы
     * \param programIndex - индекс новой программы
     * \param programName - имя новой программы
     */
    void onProgramAdded(int programIndex, QString programName);

    /*!
     * \brief onProgramDeleted вызывается сигналом TabsListWidget::programDeleted при удалении существующей программы
     * \param programIndex - индекс удаляемой программы
     * \param programName - имя удаляемой программы
     */
    void onProgramDeleted(int programIndex, QString programName);

    /*!
     * \brief onDefaultSettingsTriggered вызывается при нажатии на действие "Настройки по умолчанию"
     */
    void onDefaultSettingsTriggered();
protected:
    /*!
     * \brief programWidgetViewPushButton кнопка "Вернуться к настройке" используется на экране с отображением тела, возвращает на экран программирования
     */
    QPushButton *programWidgetViewPushButton;
};
#endif // MAINWINDOW_H
