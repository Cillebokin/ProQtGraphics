#-------------------------------------------------
#
# Project created by QtCreator 2021-07-23T14:25:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++0x
TARGET = ProQtQGraphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clascene.cpp \
    clashapeitem.cpp \
    clashapeoperate.cpp \
    clagcodeanalyse.cpp \
    dialogcuttingshape.cpp \
    clacuttingscene.cpp \
    cladxfadapter.cpp \
    clatimer.cpp \
    cladrawbyplc.cpp \
    threadgetxy.cpp \
    dialogshowwaveline.cpp

HEADERS  += mainwindow.h \
    clascene.h \
    clashapeitem.h \
    clashapeoperate.h \
    clagcodeanalyse.h \
    dialogcuttingshape.h \
    clacuttingscene.h \
    cladxfadapter.h \
    clatimer.h \
    cladrawbyplc.h \
    threadgetxy.h \
    dialogshowwaveline.h

FORMS    += mainwindow.ui \
    dialogcuttingshape.ui \
    dialogshowwaveline.ui

RESOURCES += \
    pictrues.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lX_Net
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lX_Netd
else:unix:!macx: LIBS += -L$$PWD/libs/ -lX_Net

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/libX_Net.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/libX_Netd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/X_Net.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/X_Netd.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/libs/libX_Net.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -ldxflib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -ldxflibd
else:unix:!macx: LIBS += -L$$PWD/libs/ -ldxflib

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/libdxflib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/libdxflibd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/dxflib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/dxflibd.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/libs/libdxflib.a
