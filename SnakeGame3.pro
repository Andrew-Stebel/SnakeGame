#-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T21:54:58
#
#-------------------------------------------------

INCLUDEPATH += .
QT          += core opengl widgets multimedia

TARGET = SnakeGame3
mac:CONFIG   -= app_bundle

SOURCES += main.cpp \
    camera.cpp \
    glwidget.cpp \
    window.cpp \
    drawshapes.cpp

HEADERS += \
    window.h \
    camera.h \
    glwidget.h \
    geom_types.h \
    snake.h \
    body.h \
    food.h \
    drawshapes.h \
    light.h
