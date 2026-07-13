#include <QTcpSocket>
#include <QtGlobal>
#include <QIODevice>
#include "Client.h"

//class Client : public QObject {

//   Q_OBJECT
//   QTcpSocket* socket;
//public:

Client::Client(QObject *parent) : QObject(parent) {

    socket = new QTcpSocket(this);

    QTcpSocket::connect(socket, &QTcpSocket::readyRead, this, &Client::read);
    QTcpSocket::connect(socket, &QTcpSocket::disconnected, this, &Client::clientDisconnected);
    QTcpSocket::connect(socket, &QTcpSocket::connected, this, &Client::connectedToServer);
}

Client::~Client() {}

void Client::connectToServer(const QHostAddress &serverAddress, quint16 port) {

    socket->connectToHost(serverAddress, port, QTcpSocket::ReadWrite);
    socket->waitForConnected();
    socket->write("echo");
    qDebug() << this << "says: echo" << "\n";
    socket->flush();
}

void Client::connectedToServer() {

    qDebug() << "Connected to server";
}

void Client::clientDisconnected() {

    qDebug() << "Disconnected from server";
}

void Client::read() {

    QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = dataSender->readAll();

    QString stringData = QString::fromUtf8(data);

    if (stringData.trimmed() == "ok")
        qDebug() << "Successful for " << this << "\n";
    else qDebug() << "Error" << "\n";
}

//void write() {

//    socket->write("echo");
//}
