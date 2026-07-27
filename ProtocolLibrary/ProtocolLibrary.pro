QT -= gui

TEMPLATE = lib
DEFINES += PROTOCOLLIBRARY_LIBRARY

CONFIG += c++17

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/src

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/PackageParser.cpp \
    src/SineGenerator.cpp \
    src/SineGeneratorForDouble.cpp \
    src/SineGeneratorForFloat.cpp \
    src/SineGeneratorForInt16.cpp \
    src/SineGeneratorForInt32.cpp \
    src/SineGeneratorForInt64.cpp \
    src/PackageParserForDouble.cpp \
    src/PackageParserForFloat.cpp \
    src/PackageParserForInt16.cpp \
    src/PackageParserForInt32.cpp \
    src/PackageParserForInt64.cpp

HEADERS += \
    include/SineGenerator.h \
    include/SineGeneratorForDouble.h \
    include/SineGeneratorForFloat.h \
    include/SineGeneratorForInt16.h \
    include/SineGeneratorForInt32.h \
    include/SineGeneratorForInt64.h \
    include/PackageParser.h \
    include/PackageParserForDouble.h \
    include/PackageParserForFloat.h \
    include/PackageParserForInt16.h \
    include/PackageParserForInt32.h \
    include/PackageParserForInt64.h \
    include/PackageTypeForClient.h \
    include/PackageTypeForServer.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
