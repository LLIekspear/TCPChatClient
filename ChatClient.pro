TEMPLATE = app
QT += widgets network sql multimedia xml
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += \
    Client.h \
    customStatusDialog.h \
    fileSendDialog.h \
    imageFullDialog.h \
    infoDialog.h \
    messagesColor.h \
    serverDialog.h \
    user.h \
    userInfoDialog.h \
    usernameDialog.h
    ui_Client.h

SOURCES += \
    Client.cpp \
    customStatusDialog.cpp \
    fileSendDialog.cpp \
    imageFullDialog.cpp \
    infoDialog.cpp \
    main.cpp \
    messagesColor.cpp \
    serverDialog.cpp \
    user.cpp \
    userInfoDialog.cpp \
    usernameDialog.cpp

FORMS += \
    Client.ui \
    client.ui
