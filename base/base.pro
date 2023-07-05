QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/..

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basedummy.cpp \
    framework/default_message_pump.cpp \
    framework/message_loop.cpp \
    framework/message_loop_proxy.cpp \
    framework/message_pump.cpp \
    framework/win_io_message_pump.cpp \
    framework/win_message_pump.cpp \
    framework/win_ui_message_pump.cpp \
    synchronization/lock_posix.cpp \
    synchronization/lock_win.cpp \
    synchronization/semaphore_posix.cpp \
    synchronization/semaphore_win.cpp \
    synchronization/waitable_event_posix.cpp \
    synchronization/waitable_event_win.cpp \
    thread/framework_thread.cpp \
    thread/thread_local_posix.cpp \
    thread/thread_local_win.cpp \
    thread/thread_manager.cpp \
    thread/thread_posix.cpp \
    thread/thread_win.cpp \
    time/time.cpp \
    time/time_posix.cpp \
    time/time_win.cpp

HEADERS += \
    basedummy.h \
    callback/callback.h \
    framework/default_message_pump.h \
    framework/message_loop.h \
    framework/message_loop_proxy.h \
    framework/message_pump.h \
    framework/observer_list.h \
    framework/task.h \
    framework/win_io_message_pump.h \
    framework/win_message_pump.h \
    framework/win_ui_message_pump.h \
    synchronization/lock.h \
    synchronization/semaphore.h \
    synchronization/waitable_event.h \
    thread/framework_thread.h \
    thread/thread.h \
    thread/thread_local.h \
    thread/thread_manager.h \
    time/time.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    framework/readme.txt
