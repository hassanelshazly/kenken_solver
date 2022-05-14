QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++17

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
    lib/arcconsistencysolver.cpp \
    lib/backtrackingsolver.cpp \
    lib/benchmarking.cpp \
    lib/bmarcconsistencysolver.cpp \
    lib/bmbacktrackingsolver.cpp \
    lib/bmforwardcheckingsolver.cpp \
    lib/bmheuristicarcconsistencysolver.cpp \
    lib/boardgenerator.cpp \
    lib/constraint.cpp \
    lib/forwardcheckingsolver.cpp \
    lib/heuristicarcconsistencysolver.cpp \
    lib/kenkenboard.cpp \
    lib/kenkensolver.cpp \
    main.cpp \
    ui/kenken.cpp

HEADERS += \
    lib/arcconsistencysolver.h \
    lib/backtrackingsolver.h \
    lib/benchmarking.h \
    lib/bmarcconsistencysolver.h \
    lib/bmbacktrackingsolver.h \
    lib/bmforwardcheckingsolver.h \
    lib/bmheuristicarcconsistencysolver.h \
    lib/boardgenerator.h \
    lib/forwardcheckingsolver.h \
    lib/heuristicarcconsistencysolver.h \
    lib/kenkensolver.h \
    ui/kenken.h \
    lib/constraint.h \
    lib/kenkenboard.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
