QT       += core gui printsupport serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG += resources_big

RC_ICONS = icons/reforma1.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addprogramdialog.cpp \
    bodyParts/abstractbodypart.cpp \
    bodyParts/body.cpp \
    choosingdialog.cpp \
    decoder.cpp \
    deleteprogramdialog.cpp \
    dialog.cpp \
    dotsPairs/intervaldotspair.cpp \
    dotsPairs/miosuit.cpp \
    dotsPairs/programslidersvalues.cpp \
    dotsPairs/simpledotspair.cpp \
    bodyParts/bodypart.cpp \
    dotsPairs/customsliderwidget.cpp \
    dotsPairs/popdotspair.cpp \
    graphicItems/customprogressbar.cpp \
    graphicItems/customslider.cpp \
    graphicItems/dot.cpp \
    dotsPairs/dotspair.cpp \
    bodyParts/heater.cpp \
    bodyParts/hoverpixmapitem.cpp \
    graphicItems/labeledsliderwidget.cpp \
    graphicItems/pointer.cpp \
    graphicItems/resizablegraphicsview.cpp \
    impulseadder.cpp \
    intervaldialog.cpp \
    intervalsimulation.cpp \
    intervalsimulationwidget.cpp \
    jsonclient.cpp \
    licenceDialogs/licencetext.cpp \
    licenceDialogs/licenceupdate.cpp \
    main.cpp \
    mainwindow.cpp \
    bodyParts/popbodypart.cpp \
    graphicItems/textrect.cpp \
    programwidget.cpp \
    recievedialog.cpp \
    tabswidget.cpp \
    timeaxis.cpp

HEADERS += \
    addprogramdialog.h \
    bodyParts/abstractbodypart.h \
    bodyParts/body.h \
    choosingdialog.h \
    decoder.h \
    deleteprogramdialog.h \
    dialog.h \
    dotsPairs/intervaldotspair.h \
    dotsPairs/miosuit.h \
    dotsPairs/programslidersvalues.h \
    dotsPairs/simpledotspair.h \
    config.h \
    bodyParts/bodypart.h \
    dotsPairs/customsliderwidget.h \
    dotsPairs/popdotspair.h \
    graphicItems/customprogressbar.h \
    graphicItems/customslider.h \
    graphicItems/dot.h \
    dotsPairs/dotspair.h \
    bodyParts/heater.h \
    bodyParts/hoverpixmapitem.h \
    graphicItems/labeledsliderwidget.h \
    graphicItems/pointer.h \
    graphicItems/resizablegraphicsview.h \
    impulseadder.h \
    intervaldialog.h \
    intervalsimulation.h \
    intervalsimulationwidget.h \
    jsonclient.h \
    licenceDialogs/licencetext.h \
    licenceDialogs/licenceupdate.h \
    mainwindow.h \
    bodyParts/popbodypart.h \
    graphicItems/textrect.h \
    programwidget.h \
    recievedialog.h \
    tabswidget.h \
    timeaxis.h

FORMS += \
    addprogramdialog.ui \
    choosingdialog.ui \
    deleteprogramdialog.ui \
    dialog.ui \
    dotsPairs/customsliderwidget.ui \
    graphicItems/labeledsliderwidget.ui \
    intervaldialog.ui \
    intervalsimulation.ui \
    intervalsimulationwidget.ui \
    jsonclient.ui \
    licenceDialogs/licencetext.ui \
    licenceDialogs/licenceupdate.ui \
    mainwindow.ui \
    programwidget.ui \
    recievedialog.ui \
    tabswidget.ui \
    timeaxis.ui

TRANSLATIONS += \
    Reforma_ru_RU.ts \
    Reforma_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

DISTFILES += \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0000.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0001.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0002.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0003.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0004.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0005.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0006.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0007.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0008.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0009.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0010.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0011.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0012.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0013.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0014.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0015.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0016.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0017.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0018.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0019.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0020.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0021.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0022.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0023.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0024.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0025.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0026.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0027.mioconfig \
    ../build-Reforma-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/defaults/0028.mioconfig \
    buttonBackground1.svg
