#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T18:01:08
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += network widgets
QT       += sql widgets

TARGET = consoleplc
TEMPLATE = app

DEFINES +="CON_MODBUS"
DEFINES +="QT_APP"


CONFIG(PER_PC,PER_PC|PER_PLC): DEFINES+=PER_PC
CONFIG(PER_PLC,PER_PC|PER_PLC): DEFINES+=PER_PLC

target.path = run/consoleplc
INSTALLS += target

INCLUDEPATH += GeneratedFiles

MOC_DIR = GeneratedFiles
OBJECTS_DIR = Objects
UI_DIR = GeneratedFiles
RCC_DIR = GeneratedFiles

SOURCES +=\
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
    ../3rdparty/thmodbus.cpp \
    ../3rdparty/modbus.c \
    ../3rdparty/modbus-tcp.c \
    ../3rdparty/modbus-rtu.c \
    ../3rdparty/modbus-data.c \
    main.cpp \
    Initfile.cpp \
    LoopPlc.cpp \
    Socket.cpp \
    Monitorizza.cpp \
    Update.cpp \
    Test.cpp \
    ../srcplc/source/Target.cpp \
    ../srcplc/source/Arc6012.cpp


HEADERS  += \
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
    ../srcplc/include/Arc6011_Slot.h \
    ../srcplc/include/Arc6010_Slot.h \
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
    ../3rdparty/thmodbus.h \
    ../3rdparty/modbus.h \
    ../3rdparty/modbus-private.h \
    ../3rdparty/modbus-tcp.h \
    ../3rdparty/modbus-version.h \
    ../3rdparty/modbus-tcp-private.h \
    ../3rdparty/modbus-rtu-private.h \
    ../3rdparty/modbus-rtu.h \
    ../3rdparty/config.h \
    Monitorizza.h \
    Socket.h \
    LoopPlc.h \
    Initfile.h \
    Update.h \
    Test.h \
    ../srcplc/include/Target.h \
    ../srcplc/include/Arc6012.h \
    ../srcplc/include/Arc6012_Slot.h


INCLUDEPATH += ../srcplc/include
INCLUDEPATH += ../3rdparty

CONFIG   += mobility
CONFIG   += console

MOBILITY =

OTHER_FILES += \
    ../srcplc/InitPlc \
    ../srcplc/InitPlc_220 \
    ../srcplc/camera.dat \
    ../srcplc/Oldcamera.txt \
    ../srcplc/PlcNet.ini

DISTFILES += \
    COMANDI \
    ../../../labserver/labserver.net

RESOURCES +=
