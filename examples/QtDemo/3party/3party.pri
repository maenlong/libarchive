# 让Release版本生成调试信息
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

#-----------------------------------------------------------------------

INCLUDEPATH += $$PWD/archivelib/include
DEPENDPATH += $$PWD/archivelib/include

INCLUDEPATH += $$PWD/archivelib/src
DEPENDPATH += $$PWD/archivelib/src

HEADERS  += \
    $$PWD/archivelib/include/archive.h \
    $$PWD/archivelib/include/archive_entry.h \
    $$PWD/archivelib/src/archivetestwgt.h

SOURCES += \
    $$PWD/archivelib/src/archivetestwgt.cpp

FORMS += \
    $$PWD/archivelib/src/archivetestwgt.ui

win32:{
    # 定义文件或文件夹路径
    LIB_ARCHIVE_FILE_PATH =$$PWD/archivelib/lib/windows/release/archive.lib

    # 检查文件或文件夹是否存在
    exists($$LIB_ARCHIVE_FILE_PATH) {
        message("Tips: Find the file or folder '$$LIB_ARCHIVE_FILE_PATH' link it!")
        DEFINES += HAVE_LIB_ARCHIVE
    } else {
        message("Error: The file or folder '$$LIB_ARCHIVE_FILE_PATH' does not exist!")
        DEFINES -= HAVE_LIB_ARCHIVE
    }

    contains(DEFINES, HAVE_LIB_ARCHIVE) {
        CONFIG(release, debug|release) {
            LIBS += -L$$PWD/archivelib/lib/windows/release -larchive

            dstDir = $$PWD/../build_release/
            dstDir = $$replace(dstDir, /, \\)

            sdkdir = $$PWD/archivelib/bin/windows/release/
            sdkdir = $$replace(sdkdir, /, \\)

            QMAKE_PRE_LINK  += xcopy $$sdkdir $$dstDir /e /r /q /y
            message(COPY_FILE_SDK  $$sdkdir->$$dstDir)
        } else::CONFIG(debug, debug|release): {
            LIBS += -L$$PWD/archivelib/lib/windows/debug -larchive

            dstDir = $$PWD/../build_debug/
            dstDir = $$replace(dstDir, /, \\)

            sdkdir = $$PWD/archivelib/bin/windows/debug/
            sdkdir = $$replace(sdkdir, /, \\)

            QMAKE_PRE_LINK  += xcopy $$sdkdir $$dstDir /e /r /q /y
            message(COPY_FILE_SDK  $$sdkdir->$$dstDir)
        }
    }
}



macx:{
    # 定义文件或文件夹路径
    LIB_ARCHIVE_FILE_PATH =$$PWD/archivelib/lib/mac/release/libarchive.a

    # 检查文件或文件夹是否存在
    exists($$LIB_ARCHIVE_FILE_PATH) {
        message("Tips: Find the file or folder '$$LIB_ARCHIVE_FILE_PATH' link it!")
        DEFINES += HAVE_LIB_ARCHIVE
    } else {
        message("Error: The file or folder '$$LIB_ARCHIVE_FILE_PATH' does not exist!")
        DEFINES -= HAVE_LIB_ARCHIVE
    }

    contains(DEFINES, HAVE_LIB_ARCHIVE) {
        CONFIG(release, debug|release) {
            LIBS += -L$$PWD/archivelib/lib/mac/release -larchive

            QMAKE_POST_LINK += $$quote(mkdir -p $$PWD/../build_release/$${TARGET}.app/Contents/Frameworks;)
            QMAKE_POST_LINK += $$quote(cp -a $$PWD/archivelib/lib/mac/release/* $$PWD/../build_release/$${TARGET}.app/Contents/Frameworks/;)
            message(COPY_FILE_SDK  $$PWD/archivelib/lib/mac/release/ -> $$PWD/../build_release/$${TARGET}.app/Contents/Frameworks/)

        } else::CONFIG(debug, debug|release): {
            LIBS += -L$$PWD/archivelib/lib/mac/debug -larchive

            QMAKE_POST_LINK += $$quote(mkdir -p $$PWD/../build_debug/$${TARGET}.app/Contents/Frameworks;)
            QMAKE_POST_LINK += $$quote(cp -a $$PWD/archivelib/lib/mac/debug/*  $$PWD/../build_debug/$${TARGET}.app/Contents/Frameworks/;)
            message(COPY_FILE_SDK  $$PWD/archivelib/lib/mac/debug/ -> $$PWD/../build_debug/$${TARGET}.app/Contents/Frameworks/)
        }
    }
}



unix!macx:{
    # 定义文件或文件夹路径
    LIB_ARCHIVE_FILE_PATH =$$PWD/archivelib/lib/mac/release/libarchive.a

    # 检查文件或文件夹是否存在
    exists($$LIB_ARCHIVE_FILE_PATH) {
        message("Tips: Find the file or folder '$$LIB_ARCHIVE_FILE_PATH' link it!")
        DEFINES += HAVE_LIB_ARCHIVE
    } else {
        message("Error: The file or folder '$$LIB_ARCHIVE_FILE_PATH' does not exist!")
        DEFINES -= HAVE_LIB_ARCHIVE
    }

    contains(DEFINES, HAVE_LIB_ARCHIVE) {
        CONFIG(release, debug|release) {
            LIBS += -L$$PWD/archivelib/lib/linux/release -larchive

            QMAKE_POST_LINK += $$quote(mkdir -p $$PWD/../build_release/$${TARGET}.app/Contents/Frameworks;)
            QMAKE_POST_LINK += $$quote(cp -a $$PWD/archivelib/lib/linux/release/* $$PWD/../build_release/$${TARGET}.app/Contents/Frameworks/;)
            message(COPY_FILE_SDK  $$PWD/archivelib/lib/linux/release/ -> $$PWD/../build_release/$${TARGET}.app/Contents/Frameworks/)

        } else::CONFIG(debug, debug|release): {
            LIBS += -L$$PWD/archivelib/lib/linux/debug -larchive

            QMAKE_POST_LINK += $$quote(mkdir -p $$PWD/../build_debug/$${TARGET}.app/Contents/Frameworks;)
            QMAKE_POST_LINK += $$quote(cp -a $$PWD/archivelib/lib/linux/debug/*  $$PWD/../build_debug/$${TARGET}.app/Contents/Frameworks/;)
            message(COPY_FILE_SDK  $$PWD/archivelib/lib/linux/debug/ -> $$PWD/../build_debug/$${TARGET}.app/Contents/Frameworks/)
        }
    }
}

#-----------------------------------------------------------------------


