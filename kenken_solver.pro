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
    lib/benchmarking/benchmarking.cpp \
    lib/benchmarking/bmarcconsistencysolver.cpp \
    lib/benchmarking/bmbacktrackingsolver.cpp \
    lib/benchmarking/bmforwardcheckingsolver.cpp \
    lib/benchmarking/bmheuristicfcsolver.cpp \
    lib/board/boardgenerator.cpp \
    lib/board/kenkenboard.cpp \
    lib/constraint/arithmeticconstraint.cpp \
    lib/solver/arcconsistencysolver.cpp \
    lib/solver/backtrackingsolver.cpp \
    lib/solver/forwardcheckingsolver.cpp \
    lib/solver/heuristicfcsolver.cpp \
    lib/solver/kenkensolver.cpp \
    main.cpp \
    ui/kenken.cpp

HEADERS += \
    lib/benchmarking/benchmarking.h \
    lib/benchmarking/bmarcconsistencysolver.h \
    lib/benchmarking/bmbacktrackingsolver.h \
    lib/benchmarking/bmforwardcheckingsolver.h \
    lib/benchmarking/bmheuristicfcsolver.h \
    lib/board/boardgenerator.h \
    lib/board/kenkenboard.h \
    lib/constraint/arithmeticconstraint.h \
    lib/solver/arcconsistencysolver.h \
    lib/solver/backtrackingsolver.h \
    lib/solver/forwardcheckingsolver.h \
    lib/solver/heuristicfcsolver.h \
    lib/solver/kenkensolver.h \
    ui/kenken.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
