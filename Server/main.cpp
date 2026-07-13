#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    quint16 port = 6789;

    Server* server = new Server(nullptr);
    if (!server->startServer(port))
        qDebug() << "Error" << "\n";

    return a.exec();
}
