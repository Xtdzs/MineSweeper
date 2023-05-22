QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Uncomment the following line to make your code fail to compile if it uses deprecated APIs.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/cell.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/minesweepergrid.cpp \
    src/mss.cpp \
    src/player.cpp

HEADERS += \
    h/cell.h \
    h/mainwindow.h \
    h/minesweepergrid.h \
    h/mss.h \
    h/player.h

FORMS += \
    IF/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    mine.qrc

RC_FILE += mine.rc

