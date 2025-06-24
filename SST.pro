QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    call.cpp \
    databasemanager.cpp \
    facade_call_system.cpp \
    hoverwidget.cpp \
    main.cpp \
    principal.cpp \
    requester.cpp \
    select_technician_dialog.cpp \
    session_manager.cpp \
    state.cpp \
    technician.cpp

HEADERS += \
    call.h \
    call_info.h \
    call_state.h \
    chat_message_info.h \
    databasemanager.h \
    facade_call_system.h \
    hoverwidget.h \
    principal.h \
    requester.h \
    select_technician_dialog.h \
    session_manager.h \
    state.h \
    technician.h \
    user_info.h

FORMS += \
    principal.ui \
    requester.ui \
    select_technician_dialog.ui \
    technician.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

win32 {
    RC_ICON = sst.ico
}
