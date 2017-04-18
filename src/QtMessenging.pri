INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qtmessenging.h

SOURCES += \
    $$PWD/qtmessenging.cpp

android{
    QT += androidextras

    INCLUDEPATH += $$PWD/android/src/
    HEADERS += $$PWD/android/src/qtmessengingprivate.h
    SOURCES += $$PWD/android/src/qtmessengingprivate.cpp \
                $$PWD/android/src/jni_android.cpp
    QMESSENGING_JAVASRC.path = /src/ro/wesell/messenging
    QMESSENGING_JAVASRC.files += $$files($$PWD/android/src/ro/wesell/messenging/*)
    INSTALLS += QMESSENGING_JAVASRC
    DISTFILES += \
        $$PWD/android/src/ro/wesell/messenging/QtMessengingSMS.java
}else: winrt{
    INCLUDEPATH += $$PWD/winrt/
    HEADERS += $$PWD/winrt/qtmessengingprivate.h
    SOURCES += $$PWD/winrt/qtmessengingprivate.cpp
}else: ios{
    QT += gui_private
    INCLUDEPATH += $$PWD/ios/
    HEADERS += \
        $$PWD/ios/qtmessengingprivate.h
    OBJECTIVE_SOURCES += \
        $$PWD/ios/qtmessengingprivate.mm
    LIBS += -framework Foundation \
            -framework Messages \
            -framework MessageUI
}else{
    INCLUDEPATH += $$PWD/dummy/
    HEADERS += \
        $$PWD/dummy/qtmessengingprivate.h
    SOURCES += \
        $$PWD/dummy/qtmessengingprivate.cpp
}

DISTFILES += \
    $$PWD/android/src/ro/wesell/messenging/Base64Util.java
