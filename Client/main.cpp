#include <QCoreApplication>
#include <QCommandLineParser>
#include "Client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption addressOption(QStringList() << "address", "Server IP", "address", "127.0.0.1");
    parser.addOption(addressOption);
    QCommandLineOption portOption(QStringList() << "port", "Port", "port", "6789");
    parser.addOption(portOption);

    parser.process(a);

    QString address = parser.value(addressOption);
    quint16 port = parser.value(portOption).toUShort();

    Client client(address, port);
    client.init();

    return a.exec();
}
