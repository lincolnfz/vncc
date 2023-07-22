QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../third_party/rpclib/include
INCLUDEPATH += $$PWD/../third_party/angle/include
INCLUDEPATH += $$PWD/../base/include
INCLUDEPATH += $$PWD/../third_party/openssl/include
INCLUDEPATH += $$PWD/../third_party/ffmpeg/include
INCLUDEPATH += $$PWD/../third_party/curl/include
INCLUDEPATH += $$PWD/../third_party/glm/include
INCLUDEPATH += $$PWD/../third_party/include
INCLUDEPATH += $$PWD/../third_party/sdl/include
INCLUDEPATH += $$PWD/../third_party/yjrtc

LIBS += $$PWD/../third_party/rpclib/lib/librpc.a
LIBS += $$PWD/../third_party/angle/lib/libEGL.so
LIBS += $$PWD/../third_party/angle/lib/libGLESv2.so
LIBS += $$PWD/../third_party/openssl/lib64/libssl.a
LIBS += $$PWD/../third_party/openssl/lib64/libcrypto.a
LIBS += $$PWD/../third_party/libevent/lib/libevent.a
LIBS += $$PWD/../third_party/libevent/lib/libevent_pthreads.a
#LIBS += $$PWD/../third_party/yjrtc/libyjrtc.so
LIBS += $$PWD/../third_party/sdl/lib/libSDL2.a
LIBS += $$PWD/../third_party/ffmpeg/lib/libavfilter.a
LIBS += $$PWD/../third_party/ffmpeg/lib/libavformat.a
LIBS += $$PWD/../third_party/ffmpeg/lib/libavcodec.a
LIBS += $$PWD/../third_party/ffmpeg/lib/libavutil.a
LIBS += $$PWD/../third_party/ffmpeg/lib/libswresample.a
LIBS += $$PWD/../third_party/ffmpeg/lib/libswscale.a
LIBS += $$PWD/../third_party/ffmpeg/lib/libavdevice.a
LIBS += -ldl
LIBS += -lz
LIBS += -latomic

#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "swscale.lib")

DESTDIR = $$PWD/../../qilinout

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
    ../base/algorithm/Base64.cpp \
    ../base/algorithm/CMD5Ex.cpp \
    ../base/algorithm/HMAC_SHA1.cpp \
    ../base/algorithm/RSASign.cpp \
    ../base/algorithm/SHA1.cpp \
    ../base/algorithm/des.cpp \
    ../base/algorithm/ehash.cpp \
    ../base/algorithm/hmac_sh1.cpp \
    ../base/basedummy.cpp \
    ../base/event/pevents.cpp \
    ../base/misctool.cpp \
    ../base/third_party/convert_utf/ConvertUTF.c \
    ../base/thread/thread.cpp \
    ../base/thread/threadmanager.cpp \
    ../base/time/time.cpp \
    ../base/time/time_posix.cpp \
    ../base/time/time_win.cpp \
    ../base/util/string_util.cpp \
    ../base/uuid/clear.c \
    ../base/uuid/compare.c \
    ../base/uuid/copy.c \
    ../base/uuid/gen_uuid.c \
    ../base/uuid/isnull.c \
    ../base/uuid/pack.c \
    ../base/uuid/parse.c \
    ../base/uuid/randutils.c \
    ../base/uuid/unpack.c \
    ../base/uuid/unparse.c \
    ../base/uuid/uuid_time.c \
    ../common/eAVClient.cpp \
    ../common/eInstructions.cpp \
    ../common/evideorender.cpp \
    ../common/qt/ecolbtn.cpp \
    ../common/qt/ecustomtoast.cpp \
    ../common/qt/eimagebtn.cpp \
    ../common/qt/emousehover.cpp \
    eAVRender.cpp \
    ePrivateProto.cpp \
    econtroltrans.cpp \
    edockerwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../base/algorithm/Base64.h \
    ../base/algorithm/CMD5Ex.h \
    ../base/algorithm/HMAC_SHA1.h \
    ../base/algorithm/RSASign.h \
    ../base/algorithm/SHA1.h \
    ../base/algorithm/des.h \
    ../base/algorithm/ehash.h \
    ../base/algorithm/hmac_sh1.h \
    ../base/base.h \
    ../base/base_config.h \
    ../base/base_export.h \
    ../base/base_types.h \
    ../base/basedummy.h \
    ../base/build/build_config.h \
    ../base/build_config.h \
    ../base/callback/callback.h \
    ../base/compiler_specific.h \
    ../base/error.h \
    ../base/event/pevents.h \
    ../base/include/json.hpp \
    ../base/jsonext.hpp \
    ../base/macros.h \
    ../base/misctool.h \
    ../base/port.h \
    ../base/third_party/convert_utf/ConvertUTF.h \
    ../base/thread/thread.h \
    ../base/thread/threadmanager.h \
    ../base/threadpool.h \
    ../base/time/time.h \
    ../base/util/string_util.h \
    ../base/util/unicode.h \
    ../base/uuid/all-io.h \
    ../base/uuid/c.h \
    ../base/uuid/randutils.h \
    ../base/uuid/uuid.h \
    ../base/uuid/uuidP.h \
    ../base/uuid/uuidd.h \
    ../common/eAVClient.h \
    ../common/eInstructions.h \
    ../common/evideorender.h \
    ../common/qt/ecolbtn.h \
    ../common/qt/ecustomtoast.h \
    ../common/qt/eimagebtn.h \
    ../common/qt/emousehover.h \
    eAVRender.h \
    ePrivateProto.h \
    econtroltrans.h \
    edockerwidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../base/third_party/convert_utf/readme.txt
