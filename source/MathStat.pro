QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VERSION = 0.0.9.3
QMAKE_TARGET_PRODUCT = MathStat
QMAKE_TARGET_DESCRIPTION = Simple and fast Mat. Statistics
QMAKE_TARGET_COPYRIGHT = Nikita Sedakov

RC_ICONS = resources/ico/icon.ico


SOURCES += \
    DialogForms/GraphicsForms/addgraphdialog.cpp \
    DialogForms/InputsForms/createsampledialog.cpp \
    DialogForms/InputsForms/generatesampledialog.cpp \
    DialogForms/InputsForms/inputsampledialog.cpp \
    Graphics/barsgraph.cpp \
    Graphics/empericdistribfunction.cpp \
    Graphics/graphicsmanager.cpp \
    Graphics/plotobject.cpp \
    Stats/anaysisdichotomousdata.cpp \
    Stats/commonfunc.cpp \
    Stats/empirical.cpp \
    Stats/sample.cpp \
    Stats/shiftproblem.cpp \
    Stats/twosamplescaleproblem.cpp \
    Stats/twosampleshiftproblem.cpp \
    Styles/stylesheets.cpp \
    ToolWidgets/collapsibleSection.cpp \
    ToolWidgets/criterialconfig.cpp \
    ToolWidgets/criterialresult.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot/qcustomplot.cpp \
    taskmanager.cpp

HEADERS += \
    DialogForms/GraphicsForms/addgraphdialog.h \
    DialogForms/InputsForms/createsampledialog.h \
    DialogForms/InputsForms/generatesampledialog.h \
    DialogForms/InputsForms/inputsampledialog.h \
    Graphics/barsgraph.h \
    Graphics/empericdistribfunction.h \
    Graphics/graphicsmanager.h \
    Graphics/plotobject.h \
    Stats/anaysisdichotomousdata.h \
    Stats/commonfunc.h \
    Stats/empirical.h \
    Stats/sample.h \
    Stats/shiftproblem.h \
    Stats/twosamplescaleproblem.h \
    Stats/twosampleshiftproblem.h \
    Styles/colors.h \
    Styles/stylesheets.h \
    ToolWidgets/collapsibleSection.h \
    ToolWidgets/criterialconfig.h \
    ToolWidgets/criterialresult.h \
    commonstruct.h \
    core.h \
    mainwindow.h \
    qcustomplot/qcustomplot.h \
    taskmanager.h

FORMS += \
    DialogForms/GraphicsForms/addgraphdialog.ui \
    DialogForms/InputsForms/createsampledialog.ui \
    DialogForms/InputsForms/generatesampledialog.ui \
    DialogForms/InputsForms/inputsampledialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/randlib/debug/ -lRandLib

INCLUDEPATH += $$PWD/randlib/debug
DEPENDPATH += $$PWD/randlib/debug

RESOURCES += \
    resources.qrc
