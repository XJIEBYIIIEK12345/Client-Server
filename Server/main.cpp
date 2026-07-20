#include <QCoreApplication>
#include "Server.h"
#include <QRandomGenerator>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    quint16 port = 6789;

    Server server(port);

    return a.exec();
}
