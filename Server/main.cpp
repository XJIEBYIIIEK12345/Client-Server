#include <QCoreApplication>
#include "Server.h"
#include <QRandomGenerator>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption portOption (QStringList() << "port", "Port", "port", "6789");
    parser.addOption(portOption);
    parser.process(a);

    quint16 port = parser.value(portOption).toUShort();

    Server server(port);

    return a.exec();
}
