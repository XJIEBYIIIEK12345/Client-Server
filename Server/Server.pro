QT = core
QT += network

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Server.cpp \
        main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Server.h \

INCLUDEPATH += $$PWD/../ProtocolLibrary/include
INCLUDEPATH += $$PWD/../ProtocolLibrary/include/SineGenerator
INCLUDEPATH += $$PWD/../ProtocolLibrary/include/PackageParser

QMAKE_LFLAGS += -Wl,-rpath,$$OUT_PWD/../ProtocolLibrary

LIBS += -L$$OUT_PWD/../ProtocolLibrary -lProtocolLibrary

QMAKE_CFLAGS += -Werror -Wall
