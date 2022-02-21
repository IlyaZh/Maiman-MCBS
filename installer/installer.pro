TEMPLATE = aux

CONFIG += file_copies
COPIES += core translations styles platforms imageformats iconengines bearer

CONFIG(release, debug|release) {
    DESTDIR_WIN = $$PWD/packages/com.maimanelectronics.MWB/data
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $$OUT_PWD/../MWB_Release
    PWD_WIN ~= s,/,\\,g

    core.path = $$DESTDIR_WIN
    translations.path = $$DESTDIR_WIN/translations
    styles.path = $$DESTDIR_WIN/styles
    platforms.path = $$DESTDIR_WIN/platforms
    imageformats.path = $$DESTDIR_WIN/imageformats
    iconengines.path = $$DESTDIR_WIN/iconengines
    bearer.path = $$DESTDIR_WIN/bearer

    core.files += $$files($$PWD_WIN/*.dll)
    core.files += $$files($$PWD_WIN/*.xml)
    core.files += $$files($$PWD_WIN/*.exe)
    bearer.files = $$files($$PWD_WIN/bearer/*.dll)
    iconengines.files = $$files($$PWD_WIN/iconengines/*.dll)
    imageformats.files = $$files($$PWD_WIN/imageformats/*.dll)
    platforms.files = $$files($$PWD_WIN/platforms/*.dll)
    styles.files = $$files($$PWD_WIN/styles/*.dll)
    translations.files = $$files($$PWD_WIN/translations/*.qm)


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

#    copydata.commands = $(COPY_DIR) $$PWD_WIN $$DESTDIR_WIN
#    first.depends = $(first) core translations styles platforms imageformats iconengines bearer
#    export(first.depends)
#    export(copydata.commands)
#    QMAKE_EXTRA_TARGETS += first core translations styles platforms imageformats iconengines bearer



    INPUT = $$PWD/config/config.xml $$PWD/packages
#    offlineInstaller.depends = core translations styles platforms imageformats iconengines bearer# copydata
    offlineInstaller.input = INPUT
    offlineInstaller.output = $$INSTALLER_OFFLINE
#    offlineInstaller.commands = $$(QTDIR)\..\..\Tools\QtInstallerFramework\4.2\bin\binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
    offlineInstaller.commands = C:\Qt\Tools\QtInstallerFramework\4.2\bin\binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
    offlineInstaller.CONFIG += target_predeps no_link combine
    QMAKE_EXTRA_COMPILERS += offlineInstaller

    INPUT = $$PWD/config/config.xml $$PWD/packages
#    onlineInstaller.depends = core translations styles platforms imageformats iconengines bearer# copydata
    onlineInstaller.input = INPUT
    onlineInstaller.output = $$INSTALLER_ONLINE
    onlineInstaller.commands  = C:/Qt/Tools/QtInstallerFramework/4.2/bin/binarycreator --online-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
    onlineInstaller.commands += && C:/Qt/Tools/QtInstallerFramework/4.2/bin/repogen -p $$PWD/packages -i com.maimanelectronics.MWB --update $$OUT_PWD/../repository
#    onlineInstaller.commands  = $$(QTDIR)/../../Tools/QtInstallerFramework/4.2/bin/binarycreator --online-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
#    onlineInstaller.commands += && $$(QTDIR)/../../Tools/QtInstallerFramework/4.2/bin/repogen -p $$PWD/packages -i com.maimanelectronics.MWB --update $$OUT_PWD/../repository
    onlineInstaller.CONFIG += target_predeps no_link combine
    QMAKE_EXTRA_COMPILERS += onlineInstaller
}

DISTFILES += \
    packages/com.maimanelectronics.MWB/meta/package.xml \
    config/config.xml
