#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T22:22:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtclient
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    ../common/sock_help.c

HEADERS  += client.h \
    ../include/poorIRC_proto.h \
    ../include/sock_help.h

FORMS    += client.ui

win32: LIBS += -lws2_32
