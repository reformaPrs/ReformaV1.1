#include "tabswidget.h"
#include "ui_tabswidget.h"
#include "deleteprogramdialog.h"
#include <AddProgramDialog.h>
#include <MainWindow.h>

#include <bits/ios_base.h>

TabsWidget::TabsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabsWidget)
{
    ui->setupUi(this);

    ui->programWidget->setVisible(false);
    ui->adminModePanel->setVisible(false);

    connect(ui->addProgramPushButton, SIGNAL(clicked()), this, SLOT(onAddPushButton_clicked()));
    connect(ui->deleteProgramPushButton, SIGNAL(clicked()), this, SLOT(onDeletePushButton_clicked()));
    connect(ui->programWidget, SIGNAL(showClicked()), this, SLOT(onProgramShowClicked()));

    connect(ui->CPPushButton, SIGNAL(clicked()), this, SLOT(onCPPushButton_clicked()));
    connect(ui->CongenitalDisordersPushButton, SIGNAL(clicked()), this, SLOT(onCongenitalDisordersPushButton_clicked()));
    connect(ui->NeuromuscularMuscularDisordersPushButton, SIGNAL(clicked()), this, SLOT(onNeuromuscularMuscularDisordersPushButton_clicked()));
    connect(ui->SpinalDisordersPushButton, SIGNAL(clicked()), this, SLOT(onSpinalDisordersPushButton_clicked()));
    connect(ui->SVMDPushButton, SIGNAL(clicked()), this, SLOT(onSVMDPushButton_clicked()));
    connect(ui->StrokePushButton, SIGNAL(clicked()), this, SLOT(onStrokePushButton_clicked()));
    connect(ui->TCPPushButton, SIGNAL(clicked()), this, SLOT(onTCPPushButton_clicked()));
    connect(ui->OtherPushButtton, SIGNAL(clicked()), this, SLOT(onOtherPushButtton_clicked()));

    programChoosingDialog = new ChoosingDialog(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    initButtons();
}

TabsWidget::~TabsWidget()
{
    delete ui;
}

ProgramWidget *TabsWidget::getProgramWidget() const
{
    return ui->programWidget;
}

QWidget *TabsWidget::getPanelWidget() const
{
    return ui->widget_layout_horizontal;
}

QLabel *TabsWidget::getLabel() const
{
    return ui->label;
}

int TabsWidget::getCurrentProgramIndex() const
{
    return programChoosingDialog->currentIndex();
}

int TabsWidget::getNumberOfPrograms() const
{
    return programNames.count();
}

const QString TabsWidget::getCurrentProgramName() const
{
    return programChoosingDialog->currentText();
}

void TabsWidget::activateAdminMode()
{
    isAdminMode = true;
    ui->programWidget->adminMode(true);
    ui->addProgramPushButton->setVisible(true);
    ui->deleteProgramPushButton->setVisible(true);
}

void TabsWidget::deactivateAdminMode()
{
    isAdminMode = false;
    ui->programWidget->adminMode(false);
    ui->addProgramPushButton->setVisible(false);
    ui->deleteProgramPushButton->setVisible(false);
}

bool TabsWidget::readProgramFromFile(QDataStream *in, const QString &programName)
{
    try{
        ui->programWidget->readFromFile(in);
    } catch (const QFileDevice::FileError& e) {
        qDebug() << "An error occured while reading file " << programName;
        return false;
    }
    return true;
}

bool TabsWidget::checkProgramNameExist(const QString &programName) const
{
    foreach(QString item, programNames){
        if(programChoosingDialog->cutString(item).toLower() == programChoosingDialog->cutString(programName).toLower())
            return true;
    }
    return false;
}

void TabsWidget::initProgramList()
{
    programChoosingDialog->initList(this->programNames);
    connect(programChoosingDialog, SIGNAL(indexChanged(int)), this, SLOT(onProgramChanged(int)));
}

bool TabsWidget::addProgramName(const QString &programName)
{
    if(!programChoosingDialog->checkProgramNameExist(programName)){
        programNames.append(programName);
        return true;
    }
    return false;
}

void TabsWidget::translate()
{
    programChoosingDialog->translate();
    ui->programWidget->translate();
    ui->retranslateUi(this);
}

void TabsWidget::setProgramNames(const QStringList &programNames)
{
    this->programNames = programNames;
}

void TabsWidget::onAddPushButton_clicked()
{
    qDebug() << "Add program";
    AddProgramDialog dialog(this);
    int result = dialog.exec();

    QString programName;
    if(result == QDialog::Accepted){
        programName = dialog.fullProgramName.trimmed();
    }else{
        qDebug() << "Cancel";
        return;
    }

    if(checkProgramNameExist(programName)){
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/icons/icons/reforma1.ico"));
        msgBox.setText(tr("Program with name ") + programChoosingDialog->cutString(programName) + tr(" already exist."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.addButton(tr("Continue"), QMessageBox::ActionRole);
        msgBox.exec();
    }

    emit programAdded(programNames.count(), programName);
    programNames.append(programName);
}

void TabsWidget::onDeletePushButton_clicked()
{
    qDebug() << "Delete program";
    DeleteProgramDialog dialog(programNames);
    int result = dialog.exec();
    if(result == QDialog::Accepted){
        foreach (QString item, programNames) {
            if(programChoosingDialog->cutString(item) == dialog.choosedProgramName){
                int index = programNames.indexOf(item);
                programNames.removeAt(index);
                emit programDeleted(index, item);
                return;
            }
        }
    }else{
        qDebug() << "Cancel";
        return;
    }
}

void TabsWidget::onCPPushButton_clicked()
{
    openChoosingDialog("cp");
}

void TabsWidget::onTCPPushButton_clicked()
{
    openChoosingDialog("tcp");
}

void TabsWidget::onStrokePushButton_clicked()
{
    openChoosingDialog("stroke");
}

void TabsWidget::onSVMDPushButton_clicked()
{
    openChoosingDialog("svmd");
}

void TabsWidget::onCongenitalDisordersPushButton_clicked()
{
    openChoosingDialog("congenital");
}

void TabsWidget::onNeuromuscularMuscularDisordersPushButton_clicked()
{
    openChoosingDialog("neuro");
}

void TabsWidget::onSpinalDisordersPushButton_clicked()
{
    openChoosingDialog("spinal");
}

void TabsWidget::onOtherPushButtton_clicked()
{
    openChoosingDialog("other");
}

void TabsWidget::onProgramChanged(int index)
{
    programChoosingDialog->hide();
    ui->widget_layout_horizontal->setVisible(false);
    ui->programWidget->setVisible(true);
    ui->label->setVisible(true);

    ui->label->setText(programChoosingDialog->cutString(programChoosingDialog->currentText()));
    emit programChanged(index);
}

void TabsWidget::initButtons()
{
    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for(QPushButton* button: buttons){
        QFont font = button->font();
        font.setPointSize(12);
        font.setFamily("Sans Serif");
        button->setFont(font);

        button->setMaximumSize(500, 100);

        button->setStyleSheet("QPushButton { "
                              "border-image: url(:/backgrounds/backgrounds/buttonBackground1.svg) 0 0 0 0 stretch stretch;"
                              "border-radius: 4px;"
                              "text-align: center;"
                              "font-size: 14px;}"
                              "QPushButton:hover"
                              "{"
                              "border-image: url(:/backgrounds/backgrounds/hoverButtonBackground1.svg) 0 0 0 0 stretch stretch;"
                              "}");
    }

    ui->addProgramPushButton->setStyleSheet("QPushButton"
                                            "{"
                                            "border-image: url(:/backgrounds/backgrounds/shwButtonBackground.svg) 0 0 0 0 stretch stretch;"
                                            "border-radius: 15px;"
                                            "text-align: center;"
                                            "color: white;}"
                                            "QPushButton:hover"
                                            "{"
                                            "border-image: url(:/backgrounds/backgrounds/hoverShwButtonBackground.svg) 0 0 0 0 stretch stretch;"
                                            "}");
    ui->deleteProgramPushButton->setStyleSheet("QPushButton"
                                               "{"
                                               "border-image: url(:/backgrounds/backgrounds/shwButtonBackground.svg) 0 0 0 0 stretch stretch;"
                                               "border-radius: 15px;"
                                               "text-align: center;"
                                               "color: white;}"
                                               "QPushButton:hover"
                                               "{"
                                               "border-image: url(:/backgrounds/backgrounds/hoverShwButtonBackground.svg) 0 0 0 0 stretch stretch;"
                                               "}");

    ui->addProgramPushButton->setMinimumHeight(20);
    ui->deleteProgramPushButton->setMinimumHeight(20);

    ui->label->setStyleSheet("QLabel"
                             "{"
                             "color: white;"
                             "font-size: 16px;"
                             "}");
}

void TabsWidget::openChoosingDialog(const QString& substring)
{
    programChoosingDialog->initFilteredListBySubstring(this->programNames, substring);
    connect(programChoosingDialog, SIGNAL(indexChanged(int)), this, SLOT(onProgramChanged(int)));
    programChoosingDialog->setModal(true);
    programChoosingDialog->show();
}

void TabsWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    if(ui->programWidget->isVisible()){
        ui->programWidget->setGeometry(0, 0, this->width(), this->height());
        ui->programWidget->setContentsMargins(0, 0, 0, 30);
        ui->widget_layout_horizontal->setVisible(false);
    }

    QPixmap background(":/body/bodyBlue.jpg");
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(background));
    this->setAutoFillBackground(true);
    QWidget::setPalette(palette);

    repaint();
}

void TabsWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(isAdminMode){
        ui->adminModePanel->setVisible(true);
    }
    if(ui->programWidget->isVisible()){
        return;
    }

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    QPixmap background(":/body/bodyBlue.jpg");

    int widgetWidth = width();
    int widgetHeight = height();

    QPixmap scaledBackground = background.scaled(widgetWidth, widgetHeight, Qt::KeepAspectRatio);

    int x = (widgetWidth - scaledBackground.width()) / 2;
    int y = (widgetHeight - scaledBackground.height()) / 2;
    painter.drawPixmap(x, y, scaledBackground);

    int adminPanelHeight = ui->adminModePanel->height();
    int adminPanelWidth = ui->adminModePanel->width();
    int xAdminPanel = (widgetWidth - adminPanelWidth) / 2;
    int yAdminPanel = widgetHeight - adminPanelHeight;
    ui->adminModePanel->move(xAdminPanel, yAdminPanel - 10);

    int xButtonWidget = (widgetWidth - ui->widget_layout_horizontal->width()) / 2;
    int yButtonWidget = (widgetHeight - ui->widget_layout_horizontal->height()) / 2;
    ui->widget_layout_horizontal->move(xButtonWidget, yButtonWidget);
}

void TabsWidget::onProgramShowClicked()
{
    emit showProgram(programChoosingDialog->currentIndex());
}
