QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appsettings.cpp \
    devicecommand.cpp \
    deviceled.cpp \
    devicemodel.cpp \
    devicesignedcommand.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowcontroller.cpp \
    modbus/modbus.cpp \
    model/ModelCommands.cpp \
    network/networkmodel.cpp \
    xmlreader.cpp

HEADERS += \
    appsettings.h \
    devicebinaryoption.h \
    devicecalibrationkoefs.h \
    devicecommand.h \
    deviceled.h \
    devicemodel.h \
    deviceparameter.h \
    devicesignedcommand.h \
    enums.h \
    globals.h \
    devicedescription.h \
    interfaces/DevCommand.h \
    interfaces/ModelCommands/ModelCmd.h \
    interfaces/ViewInterface.h \
    interfaces/devicemodelinterface.h \
    interfaces/modbusobserverinterface.h \
    mainwindow.h \
    mainwindowcontroller.h \
    modbus/modbus.h \
    model/ModelCommandInterface.h \
    model/ModelCommands.h \
    model/ModelInterface.h \
    network/networkmodel.h \
    xmlreader.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
