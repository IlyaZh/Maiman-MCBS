QT       += core gui network xml serialport network qml quickcontrols2
#debug:QT+=testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++2a -Wunused-parameter -Werror
#debug:CONFIG+=testcase #warn_on depend_includepath console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

VERSION = 1.0.0
TARGET = MaimanMultibench
TEMPLATE = app

CONFIG(debug, debug|release) {
    DESTDIR = $OUT_PWD/../../MWB_Debug
} else {
    DESTDIR = $OUT_PWD/../../MWB_Release
}
MOC_DIR = ../common/build/moc
RCC_DIR = ../common/build/rcc
UI_DIR = ../common/build/ui

OTHER_FILES += device_db.xml.in \
                device_gui.xml.in \
                icon.ico

unix:OBJECTS_DIR = ../common/build/o/unix
win32:OBJECTS_DIR = ../common/build/o/win
macx:OBJECTS_DIR = ../common/build/o/mac

CONFIG(release, debug|release) {
    QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$OUT_PWD/../MWB_Release
}

conf_db.input = device_db.xml.in
CONFIG(debug, debug|release) {
    conf_db.output = $OUT_PWD/../../MWB_Debug/device_db.xml
} else {
    conf_db.output = $OUT_PWD/../../MWB_Release/device_db.xml
}
QMAKE_SUBSTITUTES += conf_db

conf_gui.input = device_gui.xml.in
CONFIG(debug, debug|release) {
    conf_gui.output = $OUT_PWD/../../MWB_Debug/device_gui.xml
} else {
    conf_gui.output = $OUT_PWD/../../MWB_Release/device_gui.xml
}
QMAKE_SUBSTITUTES += conf_gui

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataThread.cpp \
    DebugMode.cpp \
    UpdatesChecker.cpp \
  components/mediator.cpp \
    device/connectionpolling.cpp \
    model/device/devicecondition.cpp \
    appsettings.cpp \
    datasource.cpp \
    device/commandsettings.cpp \
    device/device.cpp \
    device/devicemodel.cpp \
    factories/Parser.cpp \
    factories/parserworker.cpp \
    factories/treeitem.cpp \
    factories/xmlparser.cpp \
    main.cpp \
    mainwindow.cpp \
    model/device/HiddenWidget.cpp \
    modelguimediator.cpp \
    widgets/calibrationdialog.cpp \
    widgets/inlineedit.cpp \
    widgets/plusminusgroupwidget.cpp \
    widgets/plusminuswidget.cpp \
    widgets/quitdialog.cpp \
    widgets/readnounitparameterwidget.cpp \
    widgets/readparameterfactory.cpp \
    widgets/rescanprogresswidget.cpp \
    widgets/updatewidget.cpp \
    widgets/aboutdialog.cpp \
    widgets/connectionwidget.cpp \
    model/device/devicewidget.cpp \
    model/devicefactory.cpp \
    model/guifactory.cpp \
    network/SoftProtocol.cpp \
    network/networkmodel.cpp \
    network/protocols/modbusprotocol.cpp \
    network/datasourcefactory.cpp \
    network/serialdatasource.cpp \
    network/tcpdatasource.cpp \
#    tests/globaltest.cpp \
#    tests/test_modbus/test_modbus.cpp \
    absoluteToleranceCompare.cpp \
    widgets/binarywidget.cpp \
    widgets/controlwidget.cpp \
    widgets/readparameterwidget.cpp

HEADERS += \
    DataThread.h \
    DebugMode.h \
    UpdatesChecker.h \
    constants.h \
  components/mediator.hpp \
    device/connectionpolling.h \
  interfaces/pubsubinterface.h \
    model/device/devicecondition.h \
    appsettings.h \
    datasource.h \
    device/commandsettings.h \
    device/device.h \
    device/devicemodel.h \
    factories/Parser.h \
    factories/parserworker.h \
    factories/treeitem.h \
    factories/xmlparser.h \
    mainwindow.h \
    model/device/HiddenWidget.h \
  model/event.hpp \
  model/events/network.h \
    modelguimediator.h \
    network/IDataReceiver.h \
    widgets/calibrationdialog.h \
    widgets/inlineedit.h \
    widgets/plusminusgroupwidget.h \
    widgets/plusminuswidget.h \
    widgets/quitdialog.h \
    widgets/readnounitparameterwidget.h \
    widgets/readparameterfactory.h \
    widgets/rescanprogresswidget.h \
    widgets/updatewidget.h \
    widgets/aboutdialog.h \
    widgets/connectionwidget.h \
    model/device/devicepollrequest.h \
    model/device/devicewidget.h \
    model/devicefactory.h \
    model/guifactory.h \
    network/SoftProtocol.h \
    network/networkmodel.h \
    network/protocols/modbusprotocol.h \
    network/IDataSource.h \
    network/datasourcefactory.h \
    network/serialdatasource.h \
    network/tcpdatasource.h \
#    tests/globaltest.h \
#    tests/test_modbus/test_modbus.h \
    widgets/binarywidget.h \
    widgets/controlwidget.h \
    widgets/readparameterwidget.h

FORMS += \
    mainwindow.ui \
    model/device/HiddenWidget.ui \
    widgets/aboutdialog.ui \
    widgets/calibrationandlimitswidget.ui \
    widgets/calibrationdialog.ui \
    widgets/connectionwidget.ui \
    model/device/devicewidget.ui \
    widgets/binarywidget.ui \
    widgets/controlwidget.ui \
    widgets/quitdialog.ui \
    widgets/readparameterwidget.ui \
    widgets/rescanprogresswidget.ui

#debug:SUBDIRS += tests

RESOURCES += \
    resources.qrc

RC_ICONS = icon.ico

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

DISTFILES +=
