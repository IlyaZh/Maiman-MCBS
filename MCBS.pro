QT       += core gui network xml testlib concurrent serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#lessThan(QT_MAJOR_VERSION, 6): QT += serialport
#greaterThan(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += c++17 testcase #warn_on depend_includepath console
debug:CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

VERSION = 1.0.0
TARGET = MaimanMultibench
TEMPLATE = app

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DebugMode.cpp \
    model/device/devicecondition.cpp \
    updater/FileDownloader.cpp \
    updater/UpdateDownloader.cpp \
    appsettings.cpp \
    constants.cpp \
    datasource.cpp \
    device/commandsettings.cpp \
    device/device.cpp \
    device/devicemodel.cpp \
    factories/Parser.cpp \
    factories/parserworker.cpp \
    factories/treeitem.cpp \
    factories/xmlparser.cpp \
    main.cpp \
    maincontroller.cpp \
    mainfacade.cpp \
    mainwindow.cpp \
    model/device/HiddenWidget.cpp \
    updatewidget.cpp \
    widgets/aboutdialog.cpp \
    widgets/connectionwidget.cpp \
    model/device/devicewidget.cpp \
    model/devicefactory.cpp \
    model/guifactory.cpp \
    network/SoftProtocol.cpp \
    network/networkmodel.cpp \
    network/protocols/modbusprotocol.cpp \
    tests/globaltest.cpp \
    tests/test_modbus/test_modbus.cpp \
    absoluteToleranceCompare.cpp \
    widgets/binarywidget.cpp \
    widgets/controlwidget.cpp \
    widgets/readparameterwidget.cpp

HEADERS += \
    DebugMode.h \
    model/device/devicecondition.h \
    updater/FileDownloader.h \
    updater/UpdateDownloader.h \
    appsettings.h \
    constants.h \
    datasource.h \
    device/commandsettings.h \
    device/device.h \
    device/devicemodel.h \
    factories/Parser.h \
    factories/parserworker.h \
    factories/treeitem.h \
    factories/xmlparser.h \
    interfaces/ProtocolObserverInterface.h \
    maincontroller.h \
    mainfacade.h \
    mainwindow.h \
    model/device/HiddenWidget.h \
    updatewidget.h \
    widgets/aboutdialog.h \
    widgets/connectionwidget.h \
    model/device/devicepollrequest.h \
    model/device/devicewidget.h \
    model/devicefactory.h \
    model/guifactory.h \
    network/SoftProtocol.h \
    network/networkmodel.h \
    network/protocols/modbusprotocol.h \
    tests/globaltest.h \
    tests/test_modbus/test_modbus.h \
    widgets/binarywidget.h \
    widgets/controlwidget.h \
    widgets/readparameterwidget.h \
    version.h

FORMS += \
    mainwindow.ui \
    model/device/HiddenWidget.ui \
    widgets/aboutdialog.ui \
    widgets/connectionwidget.ui \
    model/device/devicewidget.ui \
    widgets/binarywidget.ui \
    widgets/controlwidget.ui \
    widgets/readparameterwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += updater/

#SUBDIRS += tests

RESOURCES += \
    resources.qrc

RC_ICONS = icon.ico

version.input = version.h.in
version.output = ../Maiman-MCBS-debug/version.h
QMAKE_SUBSTITUTES += version

