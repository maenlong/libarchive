QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


win32{
    # 定义文件或文件夹路径
    LIB_FILE_PATH =$$PWD/../../../build-libarchive-Desktop_x86_windows_msvc2017_pe_32bit-Release/libarchive/archive.lib
    # 检查文件或文件夹是否存在
    exists($$LIB_FILE_PATH) {
        message("Tips: Find the file or folder '$$LIB_FILE_PATH' link it!")
    } else {
        message("Error: The file or folder '$$LIB_FILE_PATH' does not exist!")
    }

    CONFIG(debug, debug|release) {
        DESTDIR =$$PWD/build_debug/
    } else {
        DESTDIR =$$PWD/build_release/
    }
    message( DESTDIR = $$DESTDIR )

#    INCLUDEPATH += $$PWD/../../libarchive
#    DEPENDPATH += $$PWD/../../libarchive

#    CONFIG(debug, debug|release) {
#        LIBS += -L$$PWD/../../../build-libarchive-Desktop_x86_windows_msvc2017_pe_32bit-Release/libarchive -larchive
#    } else {
#        LIBS += -L$$PWD/../../../build-libarchive-Desktop_x86_windows_msvc2017_pe_32bit-Release/libarchive -larchive
#    }

    INCLUDEPATH += $$PWD/include
    DEPENDPATH += $$PWD/include

    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/bin -larchive
    } else {
        LIBS += -L$$PWD/bin -larchive
    }
}


SOURCES += \
    main.cpp \
    mainwidget.cpp

HEADERS += \
    mainwidget.h

FORMS += \
    mainwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
