TEMPLATE = aux

CONFIG(release, debug|release) {
#    INSTALLER_OFFLINE = $$OUT_PWD/../installer_debug/MWB.offline
#    INSTALLER_ONLINE = $$OUT_PWD/../installer_debug/MWB.online
#    DESTDIR_WIN = $$PWD/packages/com.maimanelectronics.MWB/data
#    DESTDIR_WIN ~= s,/,\\,g
#    PWD_WIN = $$OUT_PWD/../MWB_Debug
#    PWD_WIN ~= s,/,\\,g

#    copydata.commands = $(COPY_DIR) $$PWD_WIN $$DESTDIR_WIN
#    first.depends = $(first) copydata
#    export(first.depends)
#    export(copydata.commands)
#    QMAKE_EXTRA_TARGETS += first copydata
#} else {
    INSTALLER_OFFLINE = $$OUT_PWD/../installer_release/MWB.offline
    INSTALLER_ONLINE = $$OUT_PWD/../installer_release/MWB.online

    DESTDIR_WIN = $$PWD/packages/com.maimanelectronics.MWB/data
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $$OUT_PWD/../MWB_Release
    PWD_WIN ~= s,/,\\,g
    copydata.commands = $(COPY_DIR) $$PWD_WIN $$DESTDIR_WIN
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata

    INPUT = $$PWD/config/config.xml $$PWD/packages
    offlineInstaller.depends = copydata
    offlineInstaller.input = INPUT
    offlineInstaller.output = $$INSTALLER_OFFLINE
#    offlineInstaller.commands = $$(QTDIR)\..\..\Tools\QtInstallerFramework\4.2\bin\binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
    offlineInstaller.commands = C:\Qt5\Tools\QtInstallerFramework\4.2\bin\binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
    offlineInstaller.CONFIG += target_predeps no_link combine
    QMAKE_EXTRA_COMPILERS += offlineInstaller


    INPUT = $$PWD/config/config.xml $$PWD/packages
    onlineInstaller.depends = copydata
    onlineInstaller.input = INPUT
    onlineInstaller.output = $$INSTALLER_ONLINE
    onlineInstaller.commands  = C:/Qt5/Tools/QtInstallerFramework/4.2/bin/binarycreator --online-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
    onlineInstaller.commands += && C:/Qt5/Tools/QtInstallerFramework/4.2/bin/repogen -p $$PWD/packages -i com.maimanelectronics.MWB --update $$OUT_PWD/../repository
#    onlineInstaller.commands  = $$(QTDIR)/../../Tools/QtInstallerFramework/4.2/bin/binarycreator --online-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
#    onlineInstaller.commands += && $$(QTDIR)/../../Tools/QtInstallerFramework/4.2/bin/repogen -p $$PWD/packages -i com.maimanelectronics.MWB --update $$OUT_PWD/../repository
    onlineInstaller.CONFIG += target_predeps no_link combine
    QMAKE_EXTRA_COMPILERS += onlineInstaller
}

DISTFILES += \
    packages/com.maimanelectronics.MWB/meta/package.xml \
    config/config.xml
