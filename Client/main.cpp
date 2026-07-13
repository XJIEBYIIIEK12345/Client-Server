#include <QCoreApplication>
#include "Client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    quint16 port = 6789;

    Client* client1 = new Client(nullptr);
    //Client* client2 = new Client(nullptr);

    client1->connectToServer(QHostAddress("127.0.0.1"), port);
    //client2->connectToServer(QHostAddress("127.0.0.1"), port);
    return a.exec();
}
