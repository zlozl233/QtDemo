QT += core network concurrent
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#引用第三方pri文件
include ($$PWD/api/api.pri)
include ($$PWD/deploy/deployment.pri)
include ($$PWD/JQLibrary/JQLibrary.pri)

INCLUDEPATH         += $$PWD/api
#跨平台条件编译
win32 {
    message("compile for win32")
}
unix:!macx{
    message("compile for unix")
#   linxu下编译成双击可执行文件
    QMAKE_LFLAGS += -no-pie
    if(contains(DEFINES,ARM)){
        message("compile for arm")    #ARM

    }else{
        message("compile for x86")    #x86
    }
}
macx {
    message("compile for macx")
}

#指定目标文件的名称。默认情况下包含的项目文件的基本名称。
TARGET              = ForwardService
#模板变量告诉qmake为这个应用程序生成哪种makefile。
TEMPLATE            = app
#指定来自moc的所有中间文件放置的目录（含Q_OBJECT宏的头文件转换成标准.h文件的存放目录）。
MOC_DIR             = temp/moc
#指定Qt资源编译器输出文件的目录（.qrc文件转换成qrc_*.h文件的存放目录）。
RCC_DIR             = temp/rcc
UI_DIR              = temp/ui
#指定所有中间文件.o（.obj）放置的目录。
OBJECTS_DIR         = temp/obj
#指定在何处放置目标文件。
DESTDIR             = bin

#编译器尽可能少得输出警告信息
CONFIG              += warn_off

#加载图标以及文件版本信息
RC_FILE += version.rc
#qt 多线程
CONFIG              +=qt thread

SOURCES += \
    main.cpp \
    ForwardMain.cpp \
    zlnetwork.cpp

HEADERS += \
    ForwardMain.h \
    crash_exception.h \
    zlnetwork.h

FORMS += \
    ForwardMain.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
