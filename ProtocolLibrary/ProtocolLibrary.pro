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
INCLUDEPATH += $$PWD/include/DataPrinter
INCLUDEPATH += $$PWD/src/DataPrinter

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/DataPrinter/DataPrinter.cpp \
    src/DataPrinter/DataPrinterForDouble.cpp \
    src/DataPrinter/DataPrinterForFloat.cpp \
    src/DataPrinter/DataPrinterForInt16.cpp \
    src/DataPrinter/DataPrinterForInt32.cpp \
    src/DataPrinter/DataPrinterForInt64.cpp \
    src/PackageDataArray.cpp \
    src/XmlProtocol.cpp \
    src/MessageProcessorForClient.cpp \
    src/MessageProcessorForServer.cpp \
    src/IProtocol.cpp \
    src/JsonProtocol.cpp \
    src/Package.cpp \
    src/PackageMetaData.cpp \
    src/SineGenerator/SineGenerator.cpp \
    src/SineGenerator/SineGeneratorForDouble.cpp \
    src/SineGenerator/SineGeneratorForFloat.cpp \
    src/SineGenerator/SineGeneratorForInt16.cpp \
    src/SineGenerator/SineGeneratorForInt32.cpp \
    src/SineGenerator/SineGeneratorForInt64.cpp \

HEADERS += \
    include/PackageDataArray.h \
    include/PackageMetaData.h \
    include/DataPrinter/DataPrinter.h \
    include/DataPrinter/DataPrinterForDouble.h \
    include/DataPrinter/DataPrinterForFloat.h \
    include/DataPrinter/DataPrinterForInt16.h \
    include/DataPrinter/DataPrinterForInt32.h \
    include/DataPrinter/DataPrinterForInt64.h \
    include/XmlProtocol.h \
    include/MessageProcessorForClient.h \
    include/MessageProcessorForServer.h \
    include/IProtocol.h \
    include/JsonProtocol.h \
    include/Package.h \
    include/PackageMetaData.h \
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
