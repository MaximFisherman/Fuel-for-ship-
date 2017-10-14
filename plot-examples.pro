#
#  QCustomPlot Plot Examples
#

QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = plot-examples
TEMPLATE = app

SOURCES += main.cpp\
    qcustomplot.cpp \
    authorization.cpp \
    db.cpp \
    mainwindow.cpp \
    dialog_add_engine.cpp \
    dialog_add_ship.cpp \
    add_course.cpp \
    add_site.cpp \
    parametr_ship.cpp

HEADERS  += \
    qcustomplot.h \
    authorization.h \
    db.h \
    global_variables.h \
    mainwindow.h \
    mainwindow.h \
    dialog_add_engine.h \
    dialog_add_ship.h \
    add_course.h \
    add_site.h \
    parametr_ship.h

FORMS    += \
    authorization.ui \
    mainwindow.ui \
    mainwindow.ui \
    dialog_add_engine.ui \
    dialog_add_ship.ui \
    add_course.ui \
    add_site.ui \
    parametr_ship.ui

