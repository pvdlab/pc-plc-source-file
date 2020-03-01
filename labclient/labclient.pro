#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T06:23:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += network widgets
QT       += sql widgets

target.path = run/labclient
INSTALLS += target

DEFINES +="QT_APP"

TARGET = labclient
TEMPLATE = app

MOC_DIR = GeneratedFiles
OBJECTS_DIR = Objects
UI_DIR = GeneratedFiles
RCC_DIR = GeneratedFiles

SOURCES +=\
    mainwindow.cpp \
    ../srcplc/source/Gas.cpp \
    ../srcplc/source/Vuoto.cpp \
    ../srcplc/source/Modulo.cpp \
    ../srcplc/source/Plc.cpp \
    ../srcplc/source/Catodo.cpp \
    ../srcplc/source/Bias.cpp \
    ../srcplc/source/Simula.cpp \
    ../srcplc/source/Segnale.cpp \
    ../srcplc/source/Resistenza.cpp \
    ../srcplc/source/Errore.cpp \
    ../srcplc/source/Timer.cpp \
    ../srcplc/source/StdAfx.cpp \
    ../srcplc/source/Arc6010.cpp \
    ../srcplc/source/Arc6011.cpp \
    ../srcplc/source/Arc6012.cpp \
    ../srcplc/source/Processo.cpp \
    ../srcplc/source/Ciclo.cpp \
    ../srcplc/source/Macchina.cpp \
    ../srcplc/source/Turbo.cpp \
    ../srcplc/source/Meccanica.cpp \
    ../srcplc/source/Tavola.cpp \
    ../srcplc/source/Baratron.cpp \
    ../srcplc/source/Penning.cpp \
    ../srcplc/source/Camera.cpp \
    ../srcplc/source/Pirani.cpp \
    qvuoto.cpp \
    qpotenza.cpp \
    qprocesso.cpp \
    qmonitorizza.cpp \
    qleek.cpp \
    qled.cpp \
    qanalogico.cpp \
    qgrafico.cpp \
    qnuovocompstep.cpp \
    qmostraeventi.cpp \
    qmodulo.cpp \
    qmostraerrore.cpp \
    main.cpp \
    qlabservernet.cpp \
    qdialogtavola.cpp \
    qconnettidb.cpp \
    cthreadclient.cpp \
    qsimula.cpp \
    qtest.cpp \
    qmiomenu.cpp \
    NetPlc.cpp \
    ../consoleplc/Initfile.cpp \
    ../srcplc/source/Target.cpp \
    DisegnoMacchina.cpp \
    initdialog.cpp \
    qsetcompoente.cpp \
    qsetting.cpp


HEADERS  += \
    mainwindow.h \
    ../srcplc/include/Vuoto.h \
    ../srcplc/include/DataFile.h \
    ../srcplc/include/MainCostanti.h \
    ../srcplc/include/Plc.h \
    ../srcplc/include/StdAfx.h \
    ../srcplc/include/Modulo.h \
    ../srcplc/include/Errore.h \
    ../srcplc/include/Bias.h \
    ../srcplc/include/Simula.h \
    ../srcplc/include/Segnale.h \
    ../srcplc/include/Resistenza.h \
    ../srcplc/include/Gas.h \
    ../srcplc/include/Catodo.h \
    ../srcplc/include/Timer.h \
    ../srcplc/include/Slot.h \
    ../srcplc/include/Arc6010.h \
    ../srcplc/include/Arc6011.h \
    ../srcplc/include/Arc6012.h \
    ../srcplc/include/Arc6011_Slot.h \
    ../srcplc/include/Arc6010_Slot.h \
    ../srcplc/include/Arc6012_Slot.h \
    ../srcplc/include/Processo.h \
    ../srcplc/include/Ciclo.h \
    ../srcplc/include/Macchina.h \
    ../srcplc/include/Turbo.h \
    ../srcplc/include/Meccanica.h \
    ../srcplc/include/Tavola.h \
    ../srcplc/include/Baratron.h \
    ../srcplc/include/Penning.h \
    ../srcplc/include/Camera.h \
    ../srcplc/include/Pirani.h \
    ../srcplc/include/Formati.h \
    qvuoto.h \
    qpotenza.h \
    qprocesso.h \
    qmonitorizza.h \
    qleek.h \
    qled.h \
    qcostanti.h \
    qanalogico.h \
    qnuovocompstep.h \
    qmostraeventi.h \
    qgraficoriquadro.h \
    qmodulo.h \
    qmostraerrore.h \
    qlabservernet.h \
    qdialogtavola.h \
    qconnettidb.h \
    cthreadclient.h \
    qsimula.h \
    qtest.h \
    qmiomenu.h \
    NetPlc.h \
    ../consoleplc/Initfile.h \
    ../srcplc/include/Target.h \
    DisegnoMacchina.h \
    initdialog.h \
    qsetcompoente.h \
    qsetting.h


INCLUDEPATH += ../PvdLabPlc/srcplc
INCLUDEPATH += ../srcplc/include

FORMS    += mainwindow.ui\
    qmainplcnet.ui \
    qprocesso.ui \
    qmonitorizza.ui \
    qleek.ui \
    qnuovocompstep.ui \
    qmostraeventi.ui \
    qstartprocesso.ui \
    qmostraerrore.ui \
    scanplc.ui \
    qmstart.ui \
    qdialogtavola.ui \
    qsimula.ui \
    qtest.ui \
    qmiomenu.ui \
    initdialog.ui \
    qsetcompoente.ui \
    qsetting.ui

OTHER_FILES += \
    ../srcplc/FAR_170.dat

RESOURCES  = MiaResource.qrc

DISTFILES += \
    ../../PvdLabGestionale/SQL/ABI.sql \
    Camera_6011.csv \
    Camera_6010.dat \
    Camera_6011.dat





