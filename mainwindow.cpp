#include "mainwindow.h"
#include "licenceDialogs/licencetext.h"
#include "licenceDialogs/licenceupdate.h"
#include "qlineedit.h"
#include "ui_mainwindow.h"

constexpr int fileNameLength = 4;
constexpr int LOADING_PROGRESS_BODY = 30;
constexpr int LOADING_PROGRESS_MIO_SUIT = 60;
constexpr int LOADING_PROGRESS_RECEIVE_DIALOG = 90;
constexpr int LOADING_PROGRESS_READ_FILES = 100;

MainWindow::MainWindow(Dialog *loadingDialog, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMinimumSize(1000, 700);

    statusTickTimer = new QTimer();
    connect(statusTickTimer, SIGNAL(timeout()), this, SLOT(sendStatusTick()));
    deviceConnectionTimer = new QTimer();
    connect(deviceConnectionTimer, SIGNAL(timeout()), this, SLOT(onDeviceDisconnected()));

    loadingDialog->setLoadingValue(LOADING_PROGRESS_BODY);
    body = Body::getBodyInstance();
    connect(body, SIGNAL(clickedBackground()), this, SLOT(onBackgroundClicked()));

    loadingDialog->setLoadingValue(LOADING_PROGRESS_MIO_SUIT);
    mioSuit = MioSuit::getMioSuitInstance();

    initAppDataDir("");
    //initAppDataDir("resources");
    initFonts();
    initButtons();
    initGraphicsView();
    initMenuBar();
    initIntervalWidget();
    initTranslators();

    copyDataFromResources();

    loadingDialog->setLoadingValue(LOADING_PROGRESS_RECEIVE_DIALOG);
    recieveDialog = new RecieveDialog(this);

    copyShortcut = new QShortcut(QKeySequence::Copy, this);
    connect(copyShortcut, SIGNAL(activated()), this, SLOT(onCopyShortcut()));

    clearShortcut = new QShortcut(QKeySequence::New, this);
    connect(clearShortcut, SIGNAL(activated()), this, SLOT(onClearShortcut()));

    if(kEnableDateExpiration){
        QDate expireDate = QDate(EXPIRATION_DATE);
        QDate warningDate = QDate::currentDate().addMonths(1);

        if(expireDate < warningDate || expireDate < QDate::currentDate()){
            LicenceUpdate l(expireDate);
            l.exec();
            if(expireDate < QDate::currentDate())
                QTimer::singleShot(0, this, SLOT(close()));
        }
    }

    ui->graphicsView->setVisible(false);
    connect(ui->tabsWidget, SIGNAL(showProgram(int)), this, SLOT(onProgramShow()));
    connect(ui->tabsWidget, SIGNAL(programChanged(int)), this, SLOT(onProgramChanged(int)));
    connect(ui->tabsWidget, &TabsWidget::programAdded, this, &MainWindow::onProgramAdded);
    connect(ui->tabsWidget, &TabsWidget::programDeleted, this, &MainWindow::onProgramDeleted);

    ui->tabsWidget->language = this->language;

    loadingDialog->setLoadingValue(LOADING_PROGRESS_READ_FILES);
    readFiles();
}

MainWindow::~MainWindow()
{
    delete statusTickTimer;
    delete deviceConnectionTimer;
    delete intervalDialog;
    delete body;
    delete mioSuit;
    delete scene;
    delete ui;
}

void MainWindow::initAppDataDir(const QString& customPath){
    if(customPath.isEmpty()){
        QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        QDir dir(path);

        if(!dir.exists()){
            dir.mkpath(".");
        }

        QString subFolderName = "programsMioconfig";

        if(!dir.exists(subFolderName)){
            dir.mkpath(subFolderName);
        }

        appDataPath = dir.absoluteFilePath(subFolderName + "/");
        return;
    }
    else if(customPath == "current"){
        QDir dir = QDir::current();
        QString subFolderName = "programs";

        if(!dir.exists(subFolderName)){
            dir.mkdir(subFolderName);
        }

        appDataPath = dir.absoluteFilePath(subFolderName + "/");
        return;
    }
    else if(customPath == "resources"){
        appDataPath = ":/programs/programs/";
    }
    else{
        appDataPath = customPath;
    }

    QDir appDataDir(appDataPath);

    if (!appDataDir.exists()){
        appDataDir.mkpath(appDataPath);
    }
}

void MainWindow::initFonts()
{
    int fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/Active_Reforma.ttf");

    if (fontId != -1){
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if(!fontFamilies.empty()){
            QFont customFont = QFont(fontFamilies.first(), 10);
            QApplication::setFont(customFont);

            ui->toolButtonOpen->setFont(customFont);
            ui->toolButtonSave->setFont(customFont);
            ui->toolButtonHeat->setFont(customFont);
            ui->toolButtonSend->setFont(customFont);
            ui->toolButtonRecieve->setFont(customFont);
            ui->toolButtonPrint->setFont(customFont);
            ui->toolButtonExport->setFont(customFont);
            ui->toolButtonExit->setFont(customFont);
            ui->toolButtonInterval->setFont(customFont);
            ui->toolButtonProgramList->setFont(customFont);
        }
    }
}

void MainWindow::copyDataFromResources()
{
    QDir directory(":/programs/programs");
    QStringList programFilesInResources = directory.entryList(QStringList() << "*.mioconfig", QDir::Files);

    for(const QString &fileName : programFilesInResources){
        QFile file(":/programs/programs/" + fileName);
        QString destinationPath = appDataPath + fileName;

        if(file.copy(destinationPath)){
            qDebug() << "File" << destinationPath << "has been created and copied";
        }
        else{
            qDebug() << "An error occured while copying the file " << destinationPath;
        }
    }
}

void MainWindow::initButtons()
{
    ui->toolButtonExport->setVisible(false);

    firstHalfhourButton = new QPushButton(this);
    firstHalfhourButton->setText(tr("First\nHalfhour"));
    firstHalfhourButton->setStyleSheet("QPushButton"
                                       "{"
                                       "border-image: url(:/backgrounds/backgrounds/buttonBackground1.svg) 0 0 0 0 stretch stretch;"
                                       "border-radius: 2px;"
                                       "text-align: center;"
                                       "}"
                                       "QPushButton:checked"
                                       "{"
                                       "border-image: url(:/backgrounds/backgrounds/hoverButtonBackground1.svg) 0 0 0 0 stretch stretch;"
                                       "}");
    firstHalfhourButton->setGeometry(100, 150, 150, 40);
    firstHalfhourButton->setCheckable(true);
    firstHalfhourButton->setChecked(true);
    firstHalfhourButton->setVisible(false);

    secondHalfhourButton = new QPushButton(this);
    secondHalfhourButton->setText(tr("Second\nHalfhour"));
    secondHalfhourButton->setStyleSheet("QPushButton"
                                       "{"
                                       "border-image: url(:/backgrounds/backgrounds/buttonBackground1.svg) 0 0 0 0 stretch stretch;"
                                       "border-radius: 2px;"
                                       "text-align: center;"
                                       "}"
                                       "QPushButton:checked"
                                       "{"
                                       "border-image: url(:/backgrounds/backgrounds/hoverButtonBackground1.svg) 0 0 0 0 stretch stretch;"
                                       "}");
    secondHalfhourButton->setGeometry(100, 195, 150, 40);
    secondHalfhourButton->setCheckable(true);
    secondHalfhourButton->setVisible(false);

    halfHourButtons = new QButtonGroup(this);
    halfHourButtons->addButton(firstHalfhourButton);
    halfHourButtons->addButton(secondHalfhourButton);
    halfHourButtons->setExclusive(true);
    connect(halfHourButtons, SIGNAL(buttonToggled(QAbstractButton*,bool)), this, SLOT(onHalfhourToggled(QAbstractButton*,bool)));

    ui->toolButtonOpen->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonSave->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonHeat->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonSend->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonRecieve->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonExport->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonExit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonInterval->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButtonProgramList->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    connect(ui->toolButtonOpen, SIGNAL(clicked()), this, SLOT(onOpenClicked()));
    connect(ui->toolButtonSave, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(ui->toolButtonHeat, SIGNAL(toggled(bool)), this, SLOT(onToolButtonHeatToggled(bool)));
    connect(ui->toolButtonSend, SIGNAL(clicked()), this, SLOT(onSendClicked()));
    connect(ui->toolButtonRecieve, SIGNAL(clicked()), this, SLOT(onRecieveClicked()));
    connect(ui->toolButtonPrint, SIGNAL(clicked()), this, SLOT(onPrintClicked()));
    connect(ui->toolButtonExport, SIGNAL(clicked()), this, SLOT(onExportClicked()));
    connect(ui->toolButtonExit, SIGNAL(clicked()), this, SLOT(onExitClicked()));
    connect(ui->toolButtonInterval, SIGNAL(clicked()), this, SLOT(onIntervalWindowClicked()));
    connect(ui->toolButtonProgramList, SIGNAL(clicked()), this, SLOT(onProgramListClicked()));

    ui->toolButtonRecieve->setEnabled(false);
    ui->toolButtonSend->setEnabled(false);


}

void MainWindow::initGraphicsView()
{
    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    connect(ui->graphicsView, SIGNAL(clicked()), this, SLOT(onBackgroundClicked()));

    body->addToScene(scene);
    connect(body, SIGNAL(clickedBodyPart(int)), this, SLOT(onBodyPartClicked(int)));

    mioSuit->addToScene(scene);

    fileNameText = new QGraphicsTextItem();
    serialNumberText = new QGraphicsTextItem();
    scene->addItem(serialNumberText);
    scene->addItem(fileNameText);
    serialNumberText->setZValue(1000);
    fileNameText->setZValue(1000);
//    fileNameText->setFont(QFont("Arial", 14, QFont::Medium));
//    serialNumberText->setFont(QFont("Arial", 14, QFont::Medium));

    fileNameText->setPos(0, 890);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    programWidgetViewPushButton = new QPushButton(this);
    programWidgetViewPushButton->setText(tr("Back to the program"));
    programWidgetViewPushButton->setStyleSheet("QPushButton"
                                               "{"
                                               "border-image: url(:/backgrounds/backgrounds/buttonBackground1.svg) 0 0 0 0 stretch stretch;"
                                               "border-radius: 2px;"
                                               "text-align: center;"
                                               "}"
                                               "QPushButton:hover"
                                               "{"
                                               "border-image: url(:/backgrounds/backgrounds/hoverButtonBackground1.svg) 0 0 0 0 stretch stretch;"
                                               "}");

    intervalPushButton = new QPushButton(this);
    intervalPushButton->setText(tr("Set the interval"));
    intervalPushButton->setStyleSheet("QPushButton"
                                      "{"
                                      "border-image: url(:/backgrounds/backgrounds/buttonBackground1.svg) 0 0 0 0 stretch stretch;"
                                      "border-radius: 2px;"
                                      "text-align: center;"
                                      "}"
                                      "QPushButton:hover"
                                      "{"
                                      "border-image: url(:/backgrounds/backgrounds/hoverButtonBackground1.svg) 0 0 0 0 stretch stretch;"
                                      "}");

    connect(programWidgetViewPushButton, SIGNAL(clicked()), this, SLOT(onProgramWidgetViewClicked()));
    programWidgetViewPushButton->setVisible(false);
    programWidgetViewPushButton->setMinimumWidth(200);

    connect(intervalPushButton, SIGNAL(clicked()), this, SLOT(onIntervalClicked()));
    intervalPushButton->setVisible(false);
    intervalPushButton->setMinimumWidth(150);
}

void MainWindow::initMenuBar()
{
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenClicked()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(onSaveClicked()));
    connect(ui->actionSend, SIGNAL(triggered()), this, SLOT(onSendClicked()));
    connect(ui->actionRecieve, SIGNAL(triggered()), this, SLOT(onRecieveClicked()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(onPrintClicked()));
    connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(onExportClicked()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(onExitClicked()));
    connect(ui->actionLicence, SIGNAL(triggered()), this, SLOT(onLicenceClicked()));
    connect(ui->actionHalfhour_mode, SIGNAL(triggered()), this, SLOT(onHalfhourModeEnabled()));
    connect(ui->actionAdmin, SIGNAL(triggered()), this, SLOT(onAdminModeChanged()));
    connect(ui->actionDefaultSettings, SIGNAL(triggered()), this, SLOT(onDefaultSettingsTriggered()));

    ui->actionHalfhour_mode->setEnabled(true);

    ui->actionRecieve->setEnabled(false);
    ui->actionSend->setEnabled(false);

    initPortsMenu();

    actionEN = new QAction();
    actionRU = new QAction();
    actionEN->setText(tr("English"));
    actionRU->setText(tr("Russian"));
    actionEN->setIcon(QIcon(":/icons/icons/USA_flag.png"));
    actionRU->setIcon(QIcon(":/icons/icons/RU_flag.png"));
    actionEN->setCheckable(true);
    actionRU->setCheckable(true);

    langGroup = new QActionGroup(this);

    langGroup->addAction(actionEN);
    langGroup->addAction(actionRU);
    langGroup->setExclusive(true);

    ui->menuLanguage->addAction(actionRU);
    ui->menuLanguage->addAction(actionEN);
    connect(actionEN, SIGNAL(triggered()), this, SLOT(onLanguageTriggered()));
    connect(actionRU, SIGNAL(triggered()), this, SLOT(onLanguageTriggered()));
}

void MainWindow::initPortsMenu()
{
    portsMenu = new QMenu();
    portsMenu->setTitle(tr("Ports"));

    ui->menuDevice->addMenu(portsMenu);
    connect(ui->menuDevice, SIGNAL(aboutToShow()), this, SLOT(onDeviceTriggered()));

    portsActionGroup = new QActionGroup(this);
    portsActionGroup->setExclusive(true);
}

void MainWindow::updatePortsList()
{
    bool isOldPortInList = false;

    portsInfoList = QSerialPortInfo::availablePorts();
    for(auto item : portActions){
        portsActionGroup->removeAction(item);
        portsMenu->removeAction(item);
        delete item;
    }

    portActions.clear();
    if (portsInfoList.isEmpty()) {
        portsMenu->setDisabled(true);
        port.close();
        portInfo = QSerialPortInfo();
        portsMenu->setTitle(tr("Ports"));
        statusTickTimer->stop();
    } else {
        portsMenu->setEnabled(true);
        for(const auto &port : portsInfoList){
            QAction *portAction = new QAction(port.portName(), this);
            portAction->setCheckable(true);
            if (port.portName() == portInfo.portName() && !portInfo.isNull()) {
                portAction->setChecked(true);
                isOldPortInList = true;
                statusTickTimer->start(TICK);
            }
            connect(portAction, SIGNAL(triggered()), this, SLOT(onPortTriggered()));
            portActions.append(portAction);
            portsActionGroup->addAction(portAction);
        }
        if (!isOldPortInList){
            port.close();
            portInfo = QSerialPortInfo();
            portsMenu->setTitle(tr("Ports"));
            statusTickTimer->stop();
        }
        portsMenu->addActions(portActions);
    }
}

void MainWindow::initIntervalWidget()
{
    intervalDialog = new IntervalDialog(this);
    connect(intervalDialog, SIGNAL(add()), this, SLOT(onIntervalAddClicked()));
    connect(intervalDialog, SIGNAL(partChanged(int)), this, SLOT(onIntervalIntaractivePartChanged(int)));
    intervalDialog->hide();
}

void MainWindow::initTranslators()
{
    if (translatorRU.load(":/i18n/Reforma_ru_RU")) {
        qDebug() << "RU Loaded";
    }
    if (translatorEN.load(":/i18n/Reforma_en_US")) {
        qDebug() << "EN Loaded";
    }
    if (translatorUZ.load(":/data/Reforma_uz_UZ")) {
        qDebug() << "UZ Loaded";
    }
    QFile file(QDir::currentPath() + localSaveFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qApp->installTranslator(&translatorRU);
        language = RU;
        actionRU->setChecked(true);
        ui->retranslateUi(this);
        updateText();
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_13);
    QString local;
    in >> local;
    if (local == "English"){
        language = EN;
        qApp->installTranslator(&translatorEN);
        ui->menuLanguage->setIcon(QIcon(":/icons/icons/USA_flag.png"));
        actionEN->setChecked(true);
    } else {
        language = RU;
        qApp->installTranslator(&translatorRU);
        ui->menuLanguage->setIcon(QIcon(":/icons/icons/RU_flag.png"));
        actionRU->setChecked(true);
    }
    ui->retranslateUi(this);
    updateText();
    file.close();
}

void MainWindow::saveCurrentLocal(QString local)
{
    QFile file(QDir::currentPath() + localSaveFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_13);
    out << local;
    file.close();
}

void MainWindow::updateText()
{
    actionEN->setText(tr("English"));
    actionRU->setText(tr("Russian"));
    firstHalfhourButton->setText(tr("First\nHalfhour"));
    secondHalfhourButton->setText(tr("Second\nHalfhour"));
    intervalPushButton->setText(tr("Set the interval"));
    programWidgetViewPushButton->setText(tr("Back to the program"));
    intervalDialog->translate();
    body->translate(language);
    mioSuit->translate(language);
    ui->tabsWidget->translate();
}

bool MainWindow::loadDataFromFile(QFile &file){    
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return false;
    }

    QByteArray c = file.peek(1);
    if (c[0] == '<'){
        qDebug() << "OldFile";
        QByteArray data = file.readAll();
        mioSuit->readFromFile(data);
        mioSuit->update();
        body->deactivateHalfhour();
        body->clear();
        body->update();
        body->update(mioSuit->getDotsPairs());

        file.close();
        return true;
    }
    QDataStream *in = new QDataStream(&file);
    in->setVersion(QDataStream::Qt_6_5);

    body->readFromFile(in);
    body->update();

    mioSuit->readFromFile(in);
    mioSuit->update();

    body->setIntaractive(Body::WHOLE_BODY);
    isIntervalActive = false;

    intervalDialog->updatePairs();

    file.close();
    delete in;
    return true;
}

void MainWindow::loadDataFromConfigFile(QDataStream *in){
    mioSuit->readInitValuesFromFile(in);
    mioSuit->update();

    delete in;

    ui->graphicsView->setVisible(false);
    ui->tabsWidget->setVisible(true);
    firstHalfhourButton->setVisible(false);
    secondHalfhourButton->setVisible(false);
}

QString MainWindow::createProgramFileName(int index)
{
    qDebug() << index;
    QString newProgramFileName = QString::number(index);
    while (newProgramFileName.length() < fileNameLength){
        newProgramFileName.insert(0, '0');
    }
    return newProgramFileName;
}

void MainWindow::onOpenClicked()
{
    if(isAdminModeActive && (!isIntervalModeActive)){
        saveCurrentProgram();
    }

    if(isIntervalModeActive){
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open file"), "",
                                                        tr("Reforma (*.mio);;All Files (*)"));

        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            while (fileName.indexOf('/') != -1) {
                fileName.remove(0, fileName.indexOf('/') + 1);
            }
            while (fileName.indexOf('\\') != -1) {
                fileName.remove(0, fileName.indexOf('\\') + 1);
            }
            fileName.remove(fileName.lastIndexOf('.'), fileName.size() - 1);
            fileNameText->setPlainText(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                                         file.errorString());
                return;
            }
            QByteArray c = file.peek(1);
            if (c[0] == '<'){
                qDebug() << "OldFile";

                QByteArray data = file.readAll();
                mioSuit->readFromFile(data);
                mioSuit->update();


                body->deactivateHalfhour();
                body->clear();
                body->update();
                body->update(mioSuit->getDotsPairs());
            } else {
                QDataStream *in = new QDataStream(&file);
                in->setVersion(QDataStream::Qt_6_5);

                body->readFromFile(in);
                body->update();

                mioSuit->readFromFile(in);
                mioSuit->update();

                delete in;
            }
            body->setIntaractive(Body::WHOLE_BODY);
            isIntervalActive = false;

            intervalDialog->updatePairs();

            file.close();

            if (mioSuit->getIsHalfhourActive()){
                onHalfhourModeEnabled();
            } else {
                onHalfhourModeDisabled();
            }
        }

        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"), "",
                                                    tr("Reforma (*.mioconfig);;All Files (*)"));

    if(fileName.isEmpty())
        return;
    else{
        QFile file(fileName);

        if(fileName.endsWith(".mioconfig")){
            if(!file.open(QIODevice::ReadOnly)){
                QMessageBox::information(this, tr("Unable to open file"),
                                        file.errorString());
                return;
            }

            QDataStream *in = new QDataStream(&file);
            in->setVersion(QDataStream::Qt_6_5);

            QString programName;
            *in >> programName;

            int numberOfPrograms = ui->tabsWidget->getNumberOfPrograms();

            if(!ui->tabsWidget->checkProgramNameExist(programName)){
                QFile::copy(fileName, appDataPath + createProgramFileName(numberOfPrograms) + ".mioconfig");
            }
            if(!ui->tabsWidget->readProgramFromFile(in, programName)){
                return;
            }

            loadDataFromConfigFile(in);

            ui->tabsWidget->getProgramWidget()->setSlidersBoundries(mioSuit->getMinSliderValues(), mioSuit->getMaxSliderValues());
            ui->tabsWidget->getProgramWidget()->setSlidersValues(mioSuit->getMaxValues());
            ui->tabsWidget->getProgramWidget()->setIsHalfhourActive(false);

            file.close();

            if(mioSuit->getIsHalfhourActive()){
                onHalfhourModeEnabled();
                if(firstHalfhourButton->isChecked()){
                    ui->tabsWidget->getProgramWidget()->setHalfHourIndex(0);
                } else{
                    ui->tabsWidget->getProgramWidget()->setHalfHourIndex(1);
                }
            } else{
                onHalfhourModeDisabled();
            }

            ui->tabsWidget->getPanelWidget()->setVisible(false);
            ui->tabsWidget->getProgramWidget()->setVisible(true);
            ui->tabsWidget->getLabel()->setVisible(true);
            ui->tabsWidget->getLabel()->setText(programName);
            return;
        }

        while (fileName.indexOf('/') != -1){
            fileName.remove(0, fileName.indexOf('/') + 1);
        }
        while (fileName.indexOf('\\') != -1){
            fileName.remove(0, fileName.indexOf('\\') + 1);
        }
        fileName.remove(fileName.lastIndexOf('.'), fileName.size() - 1);

        if(!loadDataFromFile(file)){
            return;
        }

        if(ui->tabsWidget->addProgramName(fileName)){
            QFile file(appDataPath + createProgramFileName(ui->tabsWidget->getNumberOfPrograms() - 1) + ".mioconfig");
            if(!file.open(QIODevice::WriteOnly)){
                QMessageBox::information(this, tr("Unable to open file"),
                                         file.errorString());
                return;
            }

            QDataStream *out = new QDataStream(&file);
            out->setVersion(QDataStream::Qt_6_5);

            *out << fileName;
            ProgramWidget::createConfigData(out);

            mioSuit->saveInitValuesToFile(out);

            delete out;
            file.close();
        }

        if(mioSuit->getIsHalfhourActive()){
            onHalfhourModeEnabled();
        }else{
            onHalfhourModeDisabled();
        }
    }
}

void MainWindow::saveCurrentProgram()
{
    int programIndex = ui->tabsWidget->getCurrentProgramIndex();
    qDebug() << "Current index for save " << programIndex << appDataPath + createProgramFileName(programIndex) + ".mioconfig" << " " << ui->tabsWidget->getCurrentProgramName();

    QFile file(appDataPath + createProgramFileName(programIndex) + ".mioconfig");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream *out = new QDataStream(&file);
    out->setVersion(QDataStream::Qt_6_5);

    mioSuit->saveHalfhour();
    body->saveHalfhour();
    *out << ui->tabsWidget->getCurrentProgramName();
    ui->tabsWidget->getProgramWidget()->saveToFile(out);

    mioSuit->saveInitValuesToFile(out);

    delete out;
    file.close();
}

void MainWindow::onSaveClicked()
{
    if(isIntervalModeActive){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", "Reforma (*.mio)");
        if (fileName == "") return;
        if (!fileName.endsWith(".mio")) fileName.append(".mio");
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                                         file.errorString());
                return;
            }
            QDataStream *out = new QDataStream(&file);
            out->setVersion(QDataStream::Qt_5_13);

            mioSuit->saveHalfhour();
            body->saveHalfhour();

            body->saveToFile(out);
            mioSuit->saveToFile(out);

            file.close();
            delete out;
        }

        return;
    }

    if(!ui->tabsWidget->getProgramWidget()->isVisible() && !ui->graphicsView->isVisible()){
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/icons/icons/reforma1.ico"));
        msgBox.setText(tr("There is no program to save."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.addButton(tr("Continue"), QMessageBox::ActionRole);
        msgBox.exec();
        return;
    }

    int programIndex = ui->tabsWidget->getCurrentProgramIndex();

    if(isAdminModeActive){
        saveCurrentProgram();
    }

    updateProgram();

    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/icons/icons/reforma1.ico"));
    msgBox.setText(tr("Program has been saved successfully. Do you want save it in export file?"));
    msgBox.setIcon(QMessageBox::Question);
    QPushButton *yesBtn = msgBox.addButton(tr("Yes"), QMessageBox::ActionRole);
    QPushButton *noBtn = msgBox.addButton(tr("No"), QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == (QAbstractButton *) yesBtn){
        qDebug() << "yes";
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", "Reforma (*.mioconfig)");
        if(fileName == "")
            return;
        if(!fileName.endsWith(".mioconfig"))
            fileName.append(".mioconfig");
        if(fileName.isEmpty())
            return;
        else
            QFile::copy(appDataPath + createProgramFileName(programIndex) + ".mioconfig", fileName);
    } else if (msgBox.clickedButton() == (QAbstractButton *) noBtn){
        qDebug() << "no";
    }
    return;
}

void MainWindow::onToolButtonHeatToggled(bool checked)
{
    if (checked) {
        mioSuit->hide();
        body->hide();
        mioSuit->showHeaters();
        ui->toolButtonInterval->setEnabled(false);
    } else {
        mioSuit->show();
        body->show();
        mioSuit->hideHeaters();
        ui->toolButtonInterval->setEnabled(true);
    }
}

void MainWindow::sendDataPackets(QVector<SimpleDotsPair *> dotsPairs)
{
    if (isOldVersionProtocol){
        outDataPacket[0] = 'V';
    } else {
        outDataPacket[0] = 'A';
    }
    outDataPacket[1] = 'H';
    int pairID = -1, len = -1;
    for (int i = 0; i < 4; i++){
        switch (i) {
        case 0:
            outDataPacket[2] = 0x0e;
            outDataPacket[3] = AH_PACKET_DATA_0;
            len = 17;
            pairID = 0;
            break;
        case 1:
            outDataPacket[2] = 0x0e;
            outDataPacket[3] = AH_PACKET_DATA_1;
            len = 17;
            pairID = 12;
            break;
        case 2:
            outDataPacket[2] = 0x0a;
            outDataPacket[3] = AH_PACKET_DATA_2;
            len = 13;
            pairID = 24;
            break;
        case 3:
            outDataPacket[2] = 0x0a;
            outDataPacket[3] = AH_PACKET_DATA_3;
            len = 13;
            pairID = 32;
            break;
        }
        for (int j = 4; j < len - 1; j++){
            int value = dotsPairs[pairID + j - 4]->getValue();
            if (value == 0){
                outDataPacket[j] = 0x00;
            } else {
                outDataPacket[j] = (value - 20) / 5;
                outDataPacket[j] |= 0b10000000;
            }
        }
        qDebug() << "Packet " << i << "was sended";
        outDataPacket[len - 1] = getCheckSum(outDataPacket, len - 1);
        port.write(outDataPacket, len);
    }
}

void MainWindow::sendTimingPacket(int time,int halfhour, int index)
{
    int len = 8;
    if (isOldVersionProtocol){
        outDataPacket[0] = 'V';
    } else {
        outDataPacket[0] = 'A';
    }
    outDataPacket[1] = 'H';
    outDataPacket[2] = 0x05;
    outDataPacket[3] = AH_PACKET_DATA_4;
    outDataPacket[4] = time;
    outDataPacket[5] = halfhour;
    outDataPacket[6] = index;
    outDataPacket[len - 1] = getCheckSum(outDataPacket, len - 1);
    port.write(outDataPacket, len);
}

void MainWindow::sendHalfhourData(bool isOldVersion)
{
    if (isOldVersion){
        QVector <SimpleDotsPair *> stepDotsPairs;
        for (auto pair : mioSuit->getDotsPairs()) {
            stepDotsPairs.append(new SimpleDotsPair(pair));
        }
        sendDataPackets(stepDotsPairs);
        sendTimingPacket(59, 59, 1);
        return;
    }

    mioSuit->saveHalfhour();
    if (mioSuit->getIsHalfhourActive()){
        for (int halfhourIndex = 0; halfhourIndex < 2; halfhourIndex++) {
            mioSuit->setHalfhourIndex(halfhourIndex);
            mioSuit->update();
            QVector <IntervalDotsPair *> intervalPairs;

            intervalPairs.append(mioSuit->getIntervalDotsPairs(true));
            intervalPairs.append(mioSuit->getIntervalDotsPairs(false));
            qDebug() << "Interval list size" << intervalPairs.size();

            QSet<int> timingsSet;
            timingsSet.insert(0);
            timingsSet.insert(MAX_INTERVAL);
            for (auto pair : intervalPairs) {
                timingsSet.insert(pair->getStartTime());
                timingsSet.insert(pair->getEndTime());
            }

            QList<int> timings = timingsSet.values();
            std::sort(timings.begin(), timings.end());
            for (auto time : timings){
                qDebug() << time;
            }

            QVector <DotsPair *> dotsPairs = mioSuit->getDotsPairs();
            for (int i = 0; i < timings.size() - 1; i++){
                QVector<SimpleDotsPair *> stepDotsPairs;
                for (auto pair : dotsPairs) {
                    stepDotsPairs.append(new SimpleDotsPair(pair));
                }
                for (auto pair : intervalPairs){
                    if (timings[i] >= pair->getStartTime() && timings[i] < pair->getEndTime()) {
                        stepDotsPairs[pair->getID()]->setValue(pair->getValue());
                    }
                }
                sendDataPackets(stepDotsPairs);
                sendTimingPacket(timings[i+1] - timings[i], halfhourIndex,  timings.size() - 1 - i);

                for (auto pair : stepDotsPairs) {
                    delete pair;
                }
            }
        }
        if (firstHalfhourButton->isChecked()){
            mioSuit->setHalfhourIndex(0);
        } else {
            mioSuit->setHalfhourIndex(1);
        }
        mioSuit->update();
        return;
    }

    for (int halfhourIndex = 0; halfhourIndex < 2; halfhourIndex++) {
        QVector <IntervalDotsPair *> intervalPairs;

        intervalPairs.append(mioSuit->getIntervalDotsPairs(true));
        intervalPairs.append(mioSuit->getIntervalDotsPairs(false));
        qDebug() << "Interval list size" << intervalPairs.size();

        QSet<int> timingsSet;
        timingsSet.insert(0);
        timingsSet.insert(MAX_INTERVAL);
        for (auto pair : intervalPairs) {
            timingsSet.insert(pair->getStartTime());
            timingsSet.insert(pair->getEndTime());
        }

        QList<int> timings = timingsSet.values();
        std::sort(timings.begin(), timings.end());
        for (auto time : timings){
            qDebug() << time;
        }

        QVector <DotsPair *> dotsPairs = mioSuit->getDotsPairs();
        for (int i = 0; i < timings.size() - 1; i++){
            QVector<SimpleDotsPair *> stepDotsPairs;
            for (auto pair : dotsPairs) {
                stepDotsPairs.append(new SimpleDotsPair(pair));
            }
            for (auto pair : intervalPairs){
                if (timings[i] >= pair->getStartTime() && timings[i] < pair->getEndTime()) {
                    stepDotsPairs[pair->getID()]->setValue(pair->getValue());
                }
            }
            sendDataPackets(stepDotsPairs);
            sendTimingPacket(timings[i+1] - timings[i], halfhourIndex,  timings.size() - 1 - i);

            for (auto pair : stepDotsPairs) {
                delete pair;
            }
        }
    }
}

void MainWindow::onSendClicked()
{
    onProgramShow();
    if (serialNumber > LAST_OLD_SERIAL_NUMBER) {
        sendHalfhourData(false);
    } else {
        sendHalfhourData(true);
    }

    QMessageBox msgBox;
    msgBox.setText(tr("All data was successfully sent"));
    msgBox.setWindowIcon(QIcon(":/icons/icons/reforma1.ico"));
    msgBox.exec();
}

void MainWindow::onRecieveClicked()
{
    if (serialNumber > LAST_OLD_SERIAL_NUMBER) {
        recieveDialog->setProgressValue(0);
        recieveDialog->setModal(true);
        recieveDialog->show();
    }
    sendCommand(GET_STATUS);
}

void MainWindow::onPrintClicked()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QPainter p(&printer);
        scene->render(&p);
        p.end();
    }
}

void MainWindow::onExportClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), QString(), "*.pdf");
    if (fileName == "") return;
    if (!fileName.endsWith(".pdf")) fileName.append(".pdf");

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter p(&printer);
    scene->render(&p);
    p.end();
}

void MainWindow::onExitClicked()
{
    qApp->quit();
}

void MainWindow::onIntervalWindowClicked()
{
    if(isIntervalModeActive)
        return;

    if(ui->tabsWidget->getProgramWidget()->isVisible() || programWidgetViewPushButton->isVisible()){
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":icons/icons/reforma1.ico"));
        msgBox.setText(tr("To proceed to the intervals mode, you need to close the current program. Close the current program?"));
        msgBox.setIcon(QMessageBox::Question);
        QPushButton *yesBtn = msgBox.addButton(tr("Yes"), QMessageBox::ActionRole);
        QPushButton *noBtn = msgBox.addButton(tr("No"), QMessageBox::ActionRole);
        msgBox.exec();

        if(msgBox.clickedButton() == (QAbstractButton *) yesBtn){
            if(isAdminModeActive){
                saveCurrentProgram();
            }
            updateProgram();
        }else if(msgBox.clickedButton() == (QAbstractButton *) noBtn){
            msgBox.close();
            return;
        }
    }
    ui->tabsWidget->setVisible(false);
    programWidgetViewPushButton->setVisible(false);

    if(!ui->graphicsView->isVisible())
        ui->graphicsView->setVisible(true);
    if(!isAdminModeActive)
        isAdminModeActive = true;

    isIntervalModeActive = true;
    initGraphicsView();

    ui->actionHalfhour_mode->setEnabled(isIntervalModeActive);
    body->setIsAdminModeActive(isIntervalModeActive);
    mioSuit->setIsAdminModeActive(isIntervalModeActive);
    ui->tabsWidget->getProgramWidget()->setSlidersBoundries(mioSuit->getMinSliderValues(), mioSuit->getMaxSliderValues());

    body->clear();
    body->update();
    mioSuit->clear();
    mioSuit->update();

    intervalPushButton->show();
    intervalPushButton->raise();
    intervalPushButton->move(100, 100);
}

void MainWindow::onIntervalClicked()
{
    intervalDialog->setIsAdminModeActive(isIntervalModeActive);
    intervalDialog->setModal(true);
    intervalDialog->show();
}

void MainWindow::onIntervalAddClicked()
{
    isIntervalActive = true;
}

void MainWindow::onHalfhourModeEnabled()
{
    if(isIntervalModeActive){
        mioSuit->saveHalfhour();
        body->saveHalfhour();
        mioSuit->activateHalfhour();
        body->activateHalfhour();
        if (!ui->tabsWidget->isVisible()){
            firstHalfhourButton->setVisible(true);
            secondHalfhourButton->setVisible(true);
        }
        ui->tabsWidget->getProgramWidget()->setIsHalfhourActive(true);
    }
}

void MainWindow::onHalfhourModeDisabled()
{
    firstHalfhourButton->setVisible(false);
    secondHalfhourButton->setVisible(false);
    body->setHalfhourIndex(0);
    mioSuit->setHalfhourIndex(0);
    firstHalfhourButton->setChecked(true);
    ui->tabsWidget->getProgramWidget()->setIsHalfhourActive(false);
}

void MainWindow::onHalfhourToggled(QAbstractButton *, bool checked)
{
    if (checked) {
        mioSuit->saveHalfhour();
        body->saveHalfhour();
        if (firstHalfhourButton->isChecked()){
            mioSuit->setHalfhourIndex(0);
            body->setHalfhourIndex(0);
        } else {
            mioSuit->setHalfhourIndex(1);
            body->setHalfhourIndex(1);
        }
        body->update();
        mioSuit->update();
        intervalDialog->updatePairs();
        body->setIntaractive(Body::WHOLE_BODY);
    }
    if (ui->toolButtonHeat->isChecked()){
        mioSuit->hide();
        body->hide();
    }
}

void MainWindow::onLicenceClicked()
{
    LicenceText d;
    d.exec();
}

void MainWindow::onClearShortcut(){
    qDebug() << "Clear shortcut";
    if (!isAdminModeActive) return;
    QMessageBox msgBox;
    if (mioSuit->getIsHalfhourActive()){
        msgBox.setText(tr("Do you want to clear current halfhour data?"));
    } else {
        msgBox.setText(tr("Do you want to clear data?"));
    }
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton(new QPushButton(tr("Yes")), QMessageBox::AcceptRole);
    msgBox.addButton(new QPushButton(tr("No")), QMessageBox::RejectRole);
    int ret = msgBox.exec();
    if (ret == 0){
        qDebug() << "Clear";
        body->clearHalfhour();
        mioSuit->clearHalfhour();
        intervalDialog->updatePairs();
    }
}

void MainWindow::onProgramAdded(int programIndex, QString programName){
    QFile file(appDataPath + createProgramFileName(programIndex) + ".mioconfig");
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QDataStream *out = new QDataStream(&file);
    out->setVersion(QDataStream::Qt_6_5);

    *out << programName;
    ProgramWidget::createConfigData(out);

    mioSuit->clear();
    mioSuit->saveInitValuesToFile(out);

    if(ui->tabsWidget->getProgramWidget()->isVisible()){
        ui->graphicsView->setVisible(false);
        ui->tabsWidget->getLabel()->setVisible(false);
        programWidgetViewPushButton->setVisible(false);
        ui->tabsWidget->getProgramWidget()->setVisible(false);
        ui->tabsWidget->setVisible(true);
        ui->tabsWidget->getPanelWidget()->setVisible(true);
    }

    delete out;
    file.close();
}

void MainWindow::onProgramDeleted(int programIndex, QString programName)
{
    QString filePath = appDataPath + createProgramFileName(programIndex) + ".mioconfig";

    if(QFile::remove(filePath)){
        qDebug() << "File" << programName << "has been deleted successfully!";
    }else{
        qDebug() << "An error occured while deleting the file " << programName;
        return;
    }

    QDir programsDir(appDataPath);
    QStringList files = programsDir.entryList(QDir::Files);

    for(; programIndex < files.count() - 1; programIndex++){
        QString oldFilePath = programsDir.absoluteFilePath(createProgramFileName(programIndex + 1) + ".mioconfig");
        QString newFilePath = programsDir.absoluteFilePath(createProgramFileName(programIndex) + ".mioconfig");
        programsDir.rename(oldFilePath, newFilePath);
    }
}

void MainWindow::updateProgram(){
    if (ui->tabsWidget->getProgramWidget()->getIsHalfhourActive()){
        onHalfhourModeEnabled();
        if(ui->tabsWidget->getProgramWidget()->getHalfhourIndex() == 0){
            firstHalfhourButton->setChecked(true);
            secondHalfhourButton->setChecked(false);
        } else {
            firstHalfhourButton->setChecked(false);
            secondHalfhourButton->setChecked(true);
        }
    }
    mioSuit->setProgramSlidersValues(ui->tabsWidget->getProgramWidget()->getProgramValues());

    body->clear();

    body->update();
    body->update(mioSuit->getDotsPairs());
}

void MainWindow::onProgramShow()
{
    updateProgram();
    ui->graphicsView->setVisible(true);
    programWidgetViewPushButton->show();
    programWidgetViewPushButton->raise();
    programWidgetViewPushButton->move(100, 100);
    ui->tabsWidget->setVisible(false);

    if (mioSuit->getIsHalfhourActive()){
        firstHalfhourButton->setVisible(true);
        secondHalfhourButton->setVisible(true);
    }
}

void MainWindow::onProgramChanged(int index)
{
    QFile file(appDataPath + "/" + createProgramFileName(index) + ".mioconfig");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream *in = new QDataStream(&file);
    in->setVersion(QDataStream::Qt_6_5);

    QString programName;
    *in >> programName;
    qDebug() << programName;

    ui->tabsWidget->getProgramWidget()->readFromFile(in);

    loadDataFromConfigFile(in);
    qDebug() << "File loaded";
    ui->tabsWidget->getProgramWidget()->setSlidersBoundries(mioSuit->getMinSliderValues(), mioSuit->getMaxSliderValues());
    ui->tabsWidget->getProgramWidget()->setSlidersValues(mioSuit->getMaxValues());
    ui->tabsWidget->getProgramWidget()->setIsHalfhourActive(false);

    file.close();
}

void MainWindow::onAdminModeChanged()
{
    if (!isAdminModeActive){
        bool ok;
        QString code = QInputDialog::getText(this, tr("Admin code"),
                                             tr("Enter admin code"), QLineEdit::Normal,
                                             tr(""), &ok);
        code = code.simplified();
        code.replace(" ", "");
        code.replace("-", "");

        qDebug() << code;
        if (ok && !code.isEmpty()){
            Decoder decoder;
            QDate* date = decoder.decodeDate(code.toLocal8Bit().data(), code.size());
            if (date != nullptr)
            {
                qDebug() << *date << "\n";
                if (*date != QDate::currentDate()){
                    QMessageBox::warning(this, tr("Error"), tr("Wrong code"));
                    return;
                }
                delete date;
            } else {
                QMessageBox::warning(this, tr("Error"), tr("Wrong code"));
                return;
            }
        } else {
            return;
        }
    }

    isAdminModeActive = !isAdminModeActive;
    if (isAdminModeActive){
        ui->tabsWidget->activateAdminMode();
    } else {
        ui->tabsWidget->deactivateAdminMode();
    }
    ui->actionHalfhour_mode->setEnabled(isAdminModeActive);
    body->setIsAdminModeActive(isAdminModeActive);

    mioSuit->setIsAdminModeActive(isAdminModeActive);
    ui->tabsWidget->getProgramWidget()->setSlidersBoundries(mioSuit->getMinSliderValues(), mioSuit->getMaxSliderValues());
    mioSuit->setProgramSlidersValues(ui->tabsWidget->getProgramWidget()->getProgramValues());

    body->clear();

    body->update();
    body->update(mioSuit->getDotsPairs());
    intervalDialog->setIsAdminModeActive(isAdminModeActive);
}

void MainWindow::toDefaultSettings(){
    QDir dir(appDataPath);

    if(dir.exists()){
        dir.removeRecursively();
    }
}

void MainWindow::onDefaultSettingsTriggered(){
    QMessageBox msgBox;
    msgBox.setText(tr("Do you want revert all changes?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton(new QPushButton(tr("Yes")), QMessageBox::AcceptRole);
    msgBox.addButton(new QPushButton(tr("No")), QMessageBox::RejectRole);
    int ret = msgBox.exec();
    if (ret == 0){
        QMessageBox msgBox;
        msgBox.setText(tr("Program will be closed for reversion of settings"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.addButton(new QPushButton(tr("Continue")), QMessageBox::AcceptRole);
        msgBox.exec();

        toDefaultSettings();

        qApp->quit();
    }
}

void MainWindow::onCopyShortcut()
{
    if (!isAdminModeActive) return;
    if (!mioSuit->getIsHalfhourActive()) return;
    qDebug() << "Copy shortcut";
    QMessageBox msgBox;
    msgBox.setText(tr("Do you want to copy current halfhour to other?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton(new QPushButton(tr("Yes")), QMessageBox::AcceptRole);
    msgBox.addButton(new QPushButton(tr("No")), QMessageBox::RejectRole);
    int ret = msgBox.exec();
    if (ret == 0){
        body->copyHalfhour();
        mioSuit->copyHalfhour();
    }
}

void MainWindow::onProgramListClicked()
{
    ui->graphicsView->setVisible(false);
    ui->tabsWidget->getLabel()->setVisible(false);
    programWidgetViewPushButton->setVisible(false);
    ui->tabsWidget->getProgramWidget()->setVisible(false);
    intervalPushButton->setVisible(false);
    ui->tabsWidget->setVisible(true);
    ui->tabsWidget->getPanelWidget()->setVisible(true);

    qDebug() << isIntervalModeActive;
    if(isIntervalModeActive){
        isIntervalModeActive = !isIntervalModeActive;
        firstHalfhourButton->setVisible(false);
        secondHalfhourButton->setVisible(false);
        onAdminModeChanged();
    }
}

void MainWindow::onDeviceDisconnected()
{
    ui->toolButtonRecieve->setEnabled(false);
    ui->toolButtonSend->setEnabled(false);
    ui->actionRecieve->setEnabled(false);
    ui->actionSend->setEnabled(false);
    ui->labelDeviceStatus->show();
    deviceConnectionTimer->stop();
    updatePortsList();
    serialNumberText->setPlainText("");
    intervalIndex = 0;
}

void MainWindow::onPortTriggered()
{
    QString portName;
    for(auto port : portActions){
        if(port == sender()){
            portsMenu->setTitle(tr("Ports: \"") + port->text() + "\"");
            portName = port->text();
            break;
        }
    }
    for(const auto &item : portsInfoList){
        if(item.portName() == portName){
            port.close();
            portInfo = item;
            port.setPort(item);
            port.setBaudRate(QSerialPort::Baud38400);
            port.setParity(QSerialPort::NoParity);
            port.setDataBits(QSerialPort::Data8);
            port.setStopBits(QSerialPort::OneStop);
            port.setFlowControl(QSerialPort::NoFlowControl);
            connect(&port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
            port.open(QIODevice::ReadWrite);
            statusTickTimer->start(TICK);
            break;
        }
    }
    onDeviceDisconnected();
}

void MainWindow::onDeviceTriggered()
{
    updatePortsList();
}

void MainWindow::onLanguageTriggered()
{
    if (sender() == actionEN) {
        saveCurrentLocal("English");
        ui->menuLanguage->setIcon(QIcon(":/icons/icons/USA_flag.png"));
        qApp->removeTranslator(&translatorRU);
        qApp->installTranslator(&translatorEN);
        language = EN;
    } else if (sender() == actionRU) {
        saveCurrentLocal("Russian");
        ui->menuLanguage->setIcon(QIcon(":/icons/icons/RU_flag.png"));
        qApp->removeTranslator(&translatorEN);
        qApp->installTranslator(&translatorRU);
        language = RU;
    }
    ui->tabsWidget->language = this->language;
    ui->retranslateUi(this);
    updateText();
}

void MainWindow::onIntervalIntaractivePartChanged(int index)
{
    body->setIntaractive(static_cast<Body::ActiveBodyPart>(index));
}

void MainWindow::sendStatusTick()
{
    sendCommand(STATUS_TICK);
}

void MainWindow::onReadyRead()
{
    inputBuffer.append(port.readAll());
    if (inputBuffer.indexOf('V') != -1){
        isOldVersionProtocol = true;
    } else {
        isOldVersionProtocol = false;
    }
    char firstLetter = 'A';
    if (isOldVersionProtocol) firstLetter = 'V';
    while (true) {
        if (inputBuffer.lastIndexOf(firstLetter) == 0 || inputBuffer.lastIndexOf(firstLetter) == -1) {
            return;
        }
        int len = -1;
        for (int i = 0; i < 4; i++) {
            switch (i) {
            case 0:
                if (inputBuffer.at(i) != 'A' && inputBuffer.at(i) != 'V') {
                    inputBuffer.remove(0, inputBuffer.indexOf(firstLetter));
                    qDebug() << "ERROR: Incorrect first letter";
                    return;
                }
                break;
            case 1:
                if (inputBuffer.at(i) != 'H'){
                    inputBuffer.remove(0, inputBuffer.indexOf(firstLetter, 1));
                    qDebug() << "ERROR: Incorrect second letter";
                    return;
                }
                break;
            case 2:
                len = inputBuffer.at(i) + 3;
                break;
            case 3:
                if(inputBuffer.at(len-1) != getCheckSum(inputBuffer, len-1)){
                    inputBuffer.remove(0, inputBuffer.indexOf(firstLetter, 1));
                    qDebug() << "ERROR wrong checksum";
                    return;
                }
                if (inputBuffer.at(3) == AH_PACKET_DATA_4){
                    if(len != 12) {
                        return;
                    }
                    int minute = inputBuffer.at(4), hour = inputBuffer.at(5);
                    Q_UNUSED(minute)
                    Q_UNUSED(hour)
                    bool isRunning = 0x01 & inputBuffer.at(6), isPaused = 0x02 & inputBuffer.at(6), isHeating = 0x04 & inputBuffer.at(6);
                    Q_UNUSED(isRunning)
                    Q_UNUSED(isPaused)
                    Q_UNUSED(isHeating)
                    uint8_t sn0 = inputBuffer.at(7), sn1 = inputBuffer.at(8), sn2 = inputBuffer.at(9), sn3 = inputBuffer.at(10);
                    serialNumber = sn0 + (sn1<<8) + (sn2<<16) + (sn3<<24);
                    serialNumberText->setPlainText("S/N " + QString::number(serialNumber));

                    ui->toolButtonRecieve->setEnabled(true);
                    ui->toolButtonSend->setEnabled(true);
                    ui->actionRecieve->setEnabled(true);
                    ui->actionSend->setEnabled(true);
                    ui->labelDeviceStatus->hide();
                    deviceConnectionTimer->start(2*TICK);
                    inputBuffer.remove(0, inputBuffer.indexOf(firstLetter, 1));
                    return;
                }
                int pairID = -1;
                switch (inputBuffer.at(3)) {
                case AH_PACKET_DATA_0:
                    if(len != 17) {
                        return;
                    }
                    pairID = 0;
                    break;
                case AH_PACKET_DATA_1:
                    if(len != 17) {
                        return;
                    }
                    pairID = 12;
                    break;
                case AH_PACKET_DATA_2:
                    if(len != 13) {
                        return;
                    }
                    pairID = 24;
                    break;
                case AH_PACKET_DATA_3:
                    if(len != 13) {
                        return;
                    }
                    pairID = 32;
                    break;
                default:
                    inputBuffer.remove(0, inputBuffer.indexOf(firstLetter, 1));
                    return;
                }
                if (serialNumber <= LAST_OLD_SERIAL_NUMBER) {
                    QVector <DotsPair *> dotsPairs = mioSuit->getDotsPairs();
                    for(int j = 4; j < len - 1; j++){
                        int value = (0b11111 & inputBuffer.at(j))*5+20;
                        bool isActive = 0b10000000 & inputBuffer.at(j);
                        Q_UNUSED(isActive)
                        if (value != dotsPairs[j+pairID-4]->getValue()) {
                            dotsPairs[j+pairID-4]->setValue(value);
                            if (value > 20)
                                dotsPairs[j+pairID-4]->show();
                            else
                                dotsPairs[j+pairID-4]->hide();
                        }
                    }
                    if (pairID == 32) {
                        onHalfhourModeDisabled();
                        body->setHalfhourIndex(0);
                        body->update(dotsPairs);
                        body->deactivateHalfhour();
                        mioSuit->deactivateHalfhour();
                        body->saveHalfhour();
                        mioSuit->saveHalfhour();
                    }
                } else {
                    recieveDialog->setProgressValue(intervalIndex);
                    for (int j = 4; j < len - 1; j++) {
                        int value = (0b11111 & inputBuffer.at(j))*5+20;
                        bool isActive = 0b10000000 & inputBuffer.at(j);
                        if (isActive) {
                            recievedValues[j+pairID-4][intervalIndex] = value;
                        } else {
                            recievedValues[j+pairID-4][intervalIndex] = 0;
                        }
                    }
                    if (pairID == 32) {
                        intervalIndex++;
                        if (intervalIndex == 40) {
                            intervalIndex = 0;
                            setRecievedValues();
                        }
                    }
                }
                inputBuffer.remove(0, inputBuffer.indexOf(firstLetter, 1));
                break;
            }
        }
    }
}

void MainWindow::setRecievedValues()
{
    body->clear();
    //Первые полчаса
    mioSuit->setRecievedValues(recievedValues);

    for (int i = 0; i < 2; i++){
        QVector<IntervalDotsPair *> pairs;
        mioSuit->setHalfhourIndex(i);
        body->setHalfhourIndex(i);
        mioSuit->update();
        body->update();
        body->update(mioSuit->getDotsPairs());
        pairs.append(mioSuit->getIntervalDotsPairs(true));
        pairs.append(mioSuit->getIntervalDotsPairs(false));
        body->update(pairs);
        body->saveHalfhour();
    }
    //TODO Проверить на блоке
    if (mioSuit->getIsHalfhourActive()){
        onHalfhourModeEnabled();
        if (firstHalfhourButton->isChecked()) {
            mioSuit->setHalfhourIndex(0);
            body->setHalfhourIndex(0);;
        } else {
            mioSuit->setHalfhourIndex(1);
            body->setHalfhourIndex(1);;
        }
    } else {
        onHalfhourModeDisabled();
    }

    mioSuit->update();
    body->update();

    intervalDialog->updatePairs();
    recieveDialog->setProgressValue(40);

}

void MainWindow::onBodyPartClicked(int partID)
{
    qDebug() << "BodyPartClicked";
    body->setIntaractive(Body::NONE);

    impulseAdder = new ImpulseAdder(scene);//Всплывающее окно

    impulseAdder->setPopBodyPart(partID);
    connect(impulseAdder, SIGNAL(done(int)), this, SLOT(onImpulseAdderDone(int)));
//    ui->actionExit->setEnabled(false);
//    ui->toolButtonExit->setEnabled(false);
}

void MainWindow::onImpulseAdderDone(int result){
    int partID = impulseAdder->getPartID();
    if (result == 1){
        if (isIntervalActive) {
            if (body->getPopStatus(partID) != AbstractBodyPart::BothDisabled){
                body->setPartStatus(partID, AbstractBodyPart::Intervals);
                int count = mioSuit->addIntervalPairs(intervalDialog->isUpActive());
                intervalDialog->addPairs(count);
            }
            mioSuit->resetDotsPairs(partID);
        } else {
            body->mergeChanges(partID);
            mioSuit->mergeChanges();
        }
    }
    delete impulseAdder;
    impulseAdder = nullptr;
    if (isIntervalActive){
        if (intervalDialog->isUpActive()){
            body->setIntaractive(Body::UP_BODY);
        } else {
            body->setIntaractive(Body::DOWN_BODY);
        }
        isIntervalActive = false;
    } else {
        body->setIntaractive(Body::WHOLE_BODY);
    }

    if(!isIntervalModeActive)
        saveCurrentProgram();
}

void MainWindow::onBackgroundClicked()
{
    if (impulseAdder != nullptr){
        impulseAdder->onBackgroundClicked();
    }
}

void MainWindow::sendCommand(Command command)
{
    commandPacket[0] = 'V';
    commandPacket[1] = 'H';
    commandPacket[2] = 0x03;
    commandPacket[3] = AH_COMMAND_PACKET;
    commandPacket[4] = command;
    commandPacket[5] = getCheckSum(commandPacket, 5);
    port.write(commandPacket, 6);
    commandPacket[0] = 'A';
    commandPacket[1] = 'H';
    commandPacket[2] = 0x03;
    commandPacket[3] = AH_COMMAND_PACKET;
    commandPacket[4] = command;
    commandPacket[5] = getCheckSum(commandPacket, 5);
    port.write(commandPacket, 6);
}

char MainWindow::getCheckSum(const char *data, int len){
    char result = 0x0f;
    for(int i = 3; i < len; i++){
        result ^= data[i];
    }
    return result;
}

void MainWindow::updateFile(const QString &fileName, const QString &data)
{
    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qDebug() << "Failed to open file:" << file.errorString();
        return;
    }

    QDataStream *out = new QDataStream(&file);
    out->setVersion(QDataStream::Qt_6_5);

    *out << data;

    ProgramWidget::createConfigData(out);

    mioSuit->saveToFile(out);
    body->saveToFile(out);

    qDebug() << fileName << " saved";

    delete out;
    file.close();
}

void MainWindow::readFiles()
{
    QDir directory(appDataPath);
    QStringList programFiles = directory.entryList(QStringList() << "*.mioconfig", QDir::Files);
    QStringList programNames;

    foreach(QString fileName, programFiles) {
        QFile file(appDataPath + fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file") + fileName,
                                     file.errorString());
            continue;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_5);

        QString programName;
        in >> programName;

        qDebug() << fileName << " - FILE: " << programName;
        programNames.append(programName);

        file.close();
    }
    ui->tabsWidget->setProgramNames(programNames);
}

void MainWindow::onProgramWidgetViewClicked()
{
    ui->graphicsView->setVisible(false);
    ui->tabsWidget->setVisible(true);
    ui->tabsWidget->getPanelWidget()->setVisible(false);
    firstHalfhourButton->setVisible(false);
    secondHalfhourButton->setVisible(false);
    isIntervalModeActive = false;

    if(mioSuit->getIsHalfhourActive()){
        if(firstHalfhourButton->isChecked()){
            ui->tabsWidget->getProgramWidget()->setHalfHourIndex(0);
        }else{
            ui->tabsWidget->getProgramWidget()->setHalfHourIndex(1);
        }
    }
    programWidgetViewPushButton->setVisible(false);
}
