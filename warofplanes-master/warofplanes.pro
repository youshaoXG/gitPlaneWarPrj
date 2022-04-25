QT += core gui
QT += multimedia # 播放音乐
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    Bullet.h \
    Control.h \
    EnemyPlane.h \
    MyPlane.h \
    Plane.h \
    Object.h

SOURCES += \
    Bullet.cpp \
    Control.cpp \
    EnemyPlane.cpp \
    main.cpp \
    MyPlane.cpp \
    Plane.cpp \
    Object.cpp

CONFIG += C++11

#RC_FILE = icon.rc
RC_ICONS = warofplanesicon.ico

DISTFILES +=

CONFIG += resources_big # 解决加载资源过大无法正常加载的问题
RESOURCES += \
    resources.qrc
