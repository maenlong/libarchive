QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/3party/3party.pri)


SOURCES += \
    main.cpp

HEADERS += \

FORMS += \



#指定uic命令将.ui文件转化成ui_*.h文件的存放的目录
UI_DIR= ./tmp/ui_file
#指定rcc命令将.qrc文件转换成qrc_*.h文件的存放目录
RCC_DIR += ./tmp/qrc_file
#指定moc命令将含Q_OBJECT的头文件转换成标准.h文件的存放目录
MOC_DIR += ./tmp/moc_file
#指定目标文件(obj)的存放目录
OBJECTS_DIR += ./tmp/obj_file


win32{
    QMAKE_CXXFLAGS += /utf-8

    CONFIG(debug, debug|release) {
        DESTDIR =$$PWD/build_debug/
    } else {
        DESTDIR =$$PWD/build_release/
    }

    message( DESTDIR = $$DESTDIR )

    # 版本信息
    #VERSION = APP_VERSION
    VERSION = 1.0.0.0
    # 图标
    #RC_ICONS = $$PWD/ui/QrcFile/Custom/appLogo.ico
    # 公司名称
    QMAKE_TARGET_COMPANY = Qt
    # 产品名称
    QMAKE_TARGET_PRODUCT = Qt
    # 文件说明
    QMAKE_TARGET_DESCRIPTION = Qt
    # 版权信息
    QMAKE_TARGET_COPYRIGHT = Copyright 2019-2025 Qt All Right Reserved.
    # 中文（简体）
    RC_LANG = 0x0004

    #开启大地址
    QMAKE_LFLAGS_WINDOWS += /LARGEADDRESSAWARE

    dstDir = $$DESTDIR
    dstDir = $$replace(dstDir, /, \\)

    binDir = $$PWD/bin/*
    binDir = $$replace(binDir, /, \\)

    qtDir = $$PWD/bin/qt-bin/*
    qtDir = $$replace(qtDir, /, \\)

    # lib和dll已经都从项目中删除了。所以只copy bin下的资源文件即可
    QMAKE_POST_LINK  += xcopy $$binDir $$dstDir /e /r /q /y
    message(COPY_FILE $$binDir->$$dstDir)

    QMAKE_POST_LINK  +=  &&  xcopy $$qtDir $$dstDir /e /r /q /y
    message(COPY_FILE $$qtDir->$$dstDir)

    QMAKE_POST_LINK += && rd $$dstDir\\qt-bin\\ /s /q
    message(DELETE_FILE $$dstDir->qt-bin)

    QMAKE_POST_LINK += && rd $$dstDir\\Frameworks\\ /s /q
    message(DELETE_FILE $$dstDir->Frameworks)
}


macx {
    CONFIG(debug, debug|release) {
        DESTDIR =$$PWD/build_debug/
    } else {
        DESTDIR =$$PWD/build_release/
    }

    message( DESTDIR = $$DESTDIR )

#    ICON = bin/appIcon.icns
#    QT += macextras
#    LIBS += -framework SystemConfiguration
    DEFINES += OBJC_OLD_DISPATCH_PROTOTYPES

#    QMAKE_POST_LINK += $$quote(cp -r $$PWD/bin/Skin $$DESTDIR$${TARGET}.app/Contents/MacOS/;)
#    QMAKE_POST_LINK += $$quote(cp -r $$PWD/bin/Sound $$DESTDIR$${TARGET}.app/Contents/MacOS/;)
#    QMAKE_POST_LINK += $$quote(cp -r $$PWD/bin/Translations $$DESTDIR$${TARGET}.app/Contents/MacOS/;)

    QMAKE_POST_LINK += $$quote(cp -a $$PWD/bin/* $$DESTDIR$${TARGET}.app/Contents/MacOS/;)
    QMAKE_POST_LINK += $$quote(find $$DESTDIR$${TARGET}.app/Contents/MacOS/ -name "*.rtf" -delete;)
    QMAKE_POST_LINK += $$quote(find $$DESTDIR$${TARGET}.app/Contents/MacOS/ -name "*.txt" -delete;)
    QMAKE_POST_LINK += $$quote(find $$DESTDIR$${TARGET}.app/Contents/MacOS/ -name "appIcon.*" -delete;)
    QMAKE_POST_LINK += $$quote(rm -rf $$DESTDIR$${TARGET}.app/Contents/MacOS/Frameworks;)
    QMAKE_POST_LINK += $$quote(rm -rf $$DESTDIR$${TARGET}.app/Contents/MacOS/qt-bin;)

    QMAKE_POST_LINK += $$quote(mkdir -p $$DESTDIR$${TARGET}.app/Contents/Frameworks;)
    QMAKE_POST_LINK += $$quote(cp -a $$PWD/bin/Frameworks/* $$DESTDIR$${TARGET}.app/Contents/Frameworks/;)



    QMAKE_INFO_PLIST = $$PWD/mac/Info.plist

    # translations_en.files = $$PWD/mac/en.lproj/InfoPlist.strings
    # translations_en.path = Contents/Resources/en.lproj
    # QMAKE_BUNDLE_DATA += translations_en
    # translations_zh.files = $$PWD/mac/zh-Hans.lproj/InfoPlist.strings
    # translations_zh.path = Contents/Resources/zh-Hans.lproj
    # QMAKE_BUNDLE_DATA += translations_zh

    QMAKE_POST_LINK  +=  $$quote(rm -f $$DESTDIR$${TARGET}.app/Contents/Resources/empty.lproj;)
}



unix!max:{
    CONFIG(debug, debug|release) {
        DESTDIR =$$PWD/build_debug/
    } else {
        DESTDIR =$$PWD/build_release/
    }

    message( DESTDIR = $$DESTDIR )

    #dstDir = $$DESTDIR
    ##dstDir = $$replace(dstDir, /, \\)
    #
    #binDir = $$PWD/bin/*
    ##binDir = $$replace(binDir, /, \\)
    #
    #qtDir = $$PWD/bin/qt-bin/*
    ##qtDir = $$replace(qtDir, /, \\)
    #
    ## lib和dll已经都从项目中删除了。所以只copy bin下的资源文件即可
    #QMAKE_POST_LINK  += cp -f -r $$binDir $$dstDir
    #message(COPY_FILE  $$binDir->$$dstDir)
    #
    #QMAKE_POST_LINK  +=  &&  cp -f -r $$qtDir $$dstDir
    #message(COPY_FILE  $$qtDir->$$dstDir)
    #
    #QMAKE_POST_LINK += && rd $$dstDir\\qt-bin\\ /s /q
    #message(DELETE_FILE  $$qtDir->$$dstDir)

    QMAKE_POST_LINK  += cp -f -r $$PWD/bin/* $$$$DESTDIR
    message(COPY_FILE  $$$$PWD/bin/*->$$DESTDIR)
}



#加入调试信息
QMAKE_CFLAGS_RELEASE += -g
QMAKE_CXXFLAGS_RELEASE += -g
#禁止优化
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2

#release在最后link时默认有"-s”参数，表示"Omit all symbol information from the output file"，因此要去掉该参
win32:!msvc{QMAKE_LFLAGS_RELEASE = -mthreads
}else{#message("msvc version")#QMAKE_CFLAGS_RELEASE = -O2 -MD -Zi#QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG#QMAKE_LFLAGS_RELEASE += /MAP#QMAKE_CFLAGS_RELEASE += /Zi#QMAKE_LFLAGS_RELEASE += /debug /opt:refQMAKE_LFLAGS_RELEASE += /MAP /DEBUG /opt:ref /INCREMENTAL:NO
}

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
#LIBS += -lDbgHelp



#指定翻译文件的存放路径
#TRANSLATIONS += $$DESTDIR/Translations/trans_zh_CN.ts
#TRANSLATIONS += $$DESTDIR/Translations/trans_zh_TW.ts
#TRANSLATIONS += $$DESTDIR/Translations/trans_en.ts


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
