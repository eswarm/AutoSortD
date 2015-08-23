TEMPLATE = app

QT += qml quick widgets androidextras

SOURCES += main.cpp \
    mainwindow.cpp \
    dirwatcher.cpp \
    androidfiledialog.cpp

RESOURCES += \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    dirwatcher.h \
    androidfiledialog.h
