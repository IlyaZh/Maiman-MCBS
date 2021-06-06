QT       += core gui serialport network xml

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
    device/devcommand.cpp \
    device/device.cpp \
    device/devicemodel.cpp \
    factories/Parser.cpp \
    factories/parseitem.cpp \
    factories/treeitem.cpp \
    factories/xmlparser.cpp \
    main.cpp \
    mainviewfacade.cpp \
    mainwindow.cpp \
    model/device/devicewidget.cpp \
    model/devicefactory.cpp \
    network/SoftProtocol.cpp \
    network/networkmodel.cpp \
    network/protocols/modbus.cpp

HEADERS += \
    appsettings.h \
    device/devcommand.h \
    device/device.h \
    device/devicemodel.h \
    enums.h \
    factories/Parser.h \
    factories/parseitem.h \
    factories/treeitem.h \
    factories/xmlparser.h \
    globals.h \
    interfaces/DevCommand.h \
    interfaces/ModelCommands/ModelCmd.h \
    interfaces/ProtocolObserverInterface.h \
    interfaces/ViewInterface.h \
    interfaces/mainwindowcontrollerinterface.h \
    mainviewfacade.h \
    mainwindow.h \
    mainwindowcontroller.h \
    model/ModelInterface.h \
    model/device/devicepollrequest.h \
    model/device/devicewidget.h \
    model/devicefactory.h \
    network/SoftProtocol.h \
    network/networkmodel.h \
    network/protocols/modbus.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
