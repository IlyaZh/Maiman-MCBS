QT       += core gui network xml testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
lessThan(QT_MAJOR_VERSION, 6): QT += serialport
greaterThan(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += c++11 testcase #warn_on depend_includepath

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
    datasource.cpp \
    device/devcommand.cpp \
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
    model/device/devicewidget.cpp \
    model/device/parameterwidget.cpp \
    model/devicefactory.cpp \
    model/guifactory.cpp \
    network/SoftProtocol.cpp \
    network/networkmodel.cpp \
    network/protocols/modbusprotocol.cpp \
    tests/globaltest.cpp \
    tests/test_modbus/datasource_mock.cpp \
    tests/test_modbus/test_modbus.cpp

HEADERS += \
    appsettings.h \
    datasource.h \
    device/devcommand.h \
    device/device.h \
    device/devicemodel.h \
    factories/Parser.h \
    factories/parserworker.h \
    factories/treeitem.h \
    factories/xmlparser.h \
    globals.h \
    interfaces/DevCommand.h \
    interfaces/IMediator.h \
    interfaces/ProtocolObserverInterface.h \
    interfaces/ViewInterface.h \
    interfaces/mainwindowcontrollerinterface.h \
    maincontroller.h \
    mainfacade.h \
    mainwindow.h \
    model/ModelInterface.h \
    model/device/devicepollrequest.h \
    model/device/devicewidget.h \
    model/device/parameterwidget.h \
    model/devicefactory.h \
    model/guifactory.h \
    network/SoftProtocol.h \
    network/networkmodel.h \
    network/protocols/modbusprotocol.h \
    tests/globaltest.h \
    tests/test_modbus/datasource_mock.h \
    tests/test_modbus/test_modbus.h

FORMS += \
    mainwindow.ui \
    model/device/devicewidget.ui \
    model/device/parameterwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
