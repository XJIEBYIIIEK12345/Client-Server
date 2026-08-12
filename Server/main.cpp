#include <QCoreApplication>
#include "Server.h"
#include <QRandomGenerator>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList protocols = {"json", "bin", "xml"};

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption portOption (QStringList() << "port", "Port (default: 6789)", "port", "6789");
    parser.addOption(portOption);
    QCommandLineOption protocolOption (QStringList() << "protocol", "Transmitted data format (json, bin, xml) (default: json)", "protocol", "json");
    parser.addOption(protocolOption);

    parser.process(a);

    quint16 port = parser.value(portOption).toUShort();
    QString protocol = parser.value(protocolOption);

    if (!protocols.contains(protocol)) {
        qDebug() << "This data format is not supported";
        return -1;
    } else {
        Server server(port, protocol);

        return a.exec();
    }
}
