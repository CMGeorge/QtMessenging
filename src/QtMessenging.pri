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
}

DISTFILES += \
    $$PWD/android/src/ro/wesell/messenging/Base64Util.java
