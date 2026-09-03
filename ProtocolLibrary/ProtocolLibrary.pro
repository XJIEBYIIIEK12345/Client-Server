QT -= gui
QT += network
QT += xml

TEMPLATE = lib
DEFINES += PROTOCOLLIBRARY_LIBRARY

CONFIG += c++17

CONFIG += link_pkgconfig

PKGCONFIG += log4cplus

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/include/SineGenerator
INCLUDEPATH += $$PWD/src/SineGenerator
INCLUDEPATH += $$PWD/include/PackageParser
INCLUDEPATH += $$PWD/src/PackageParser

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/XmlProtocol.cpp \
    src/WorkerThread.cpp \
    src/MessageProcessorForClient.cpp \
    src/MessageProcessorForServer.cpp \
    src/IProtocol.cpp \
    src/JsonProtocol.cpp \
    src/MultithreadManager.cpp \
    src/Package.cpp \
    src/PackageForDataToGenerate.cpp \
    src/PackageForGeneratedData.cpp \
    src/PackageForSignal.cpp \
    src/PackageParser/PackageParser.cpp \
    src/PackageParser/PackageParserForDouble.cpp \
    src/PackageParser/PackageParserForFloat.cpp \
    src/PackageParser/PackageParserForInt16.cpp \
    src/PackageParser/PackageParserForInt32.cpp \
    src/PackageParser/PackageParserForInt64.cpp \
    src/SineGenerator/SineGenerator.cpp \
    src/SineGenerator/SineGeneratorForDouble.cpp \
    src/SineGenerator/SineGeneratorForFloat.cpp \
    src/SineGenerator/SineGeneratorForInt16.cpp \
    src/SineGenerator/SineGeneratorForInt32.cpp \
    src/SineGenerator/SineGeneratorForInt64.cpp \

HEADERS += \
    include/XmlProtocol.h \
    include/WorkerThread.h \
    include/MessageProcessorForClient.h \
    include/MessageProcessorForServer.h \
    include/IProtocol.h \
    include/JsonProtocol.h \
    include/MultithreadManager.h \
    include/Package.h \
    include/PackageForDataToGenerate.h \
    include/PackageForGeneratedData.h \
    include/PackageForSignal.h \
    include/PackageParser/PackageParser.h \
    include/PackageParser/PackageParserForDouble.h \
    include/PackageParser/PackageParserForFloat.h \
    include/PackageParser/PackageParserForInt16.h \
    include/PackageParser/PackageParserForInt32.h \
    include/PackageParser/PackageParserForInt64.h \
    include/SineGenerator/SineGenerator.h \
    include/SineGenerator/SineGeneratorForDouble.h \
    include/SineGenerator/SineGeneratorForFloat.h \
    include/SineGenerator/SineGeneratorForInt16.h \
    include/SineGenerator/SineGeneratorForInt32.h \
    include/SineGenerator/SineGeneratorForInt64.h \

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
