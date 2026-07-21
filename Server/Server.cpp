#include "Server.h"
#include <QRandomGenerator64>
#include <QDataStream>
#include <QList>
#include <iostream>
#include <QtEndian>
#include "PackageType.h"

typedef int SineValue;

Server::Server(quint16 port, QObject *parent) : QTcpServer(parent) {

    if (!listen(QHostAddress::Any, port)) {
        qDebug() << "Server was not started\n";
    } else {
        qDebug() << "Server was started on port:" << port << "\n";
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);

    if (socket->setSocketDescriptor(socketDescriptor)) {
        connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
        connect(socket, &QTcpSocket::errorOccurred, this, &Server::onErrorOccurred);
        connect(socket, &QTcpSocket::stateChanged, this, &Server::onStateChanged);
        connectedClients.insert(socket, ClientData(connectedClients.size() + 1));
    } else {
        delete socket;
    }

    qDebug() << "Client" << connectedClients[socket].index << " was connected";

    this->sendRandBytesToClient(socket);
}

void Server::onReadyRead() {

    QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
    if (!dataSender) return;

    QByteArray& buffer = connectedClients[dataSender].m_buffer;

    QByteArray data = dataSender->readAll();

    buffer.append(data);
    int count = buffer.size() / sizeof(SineValue);

    SineValue* sinus = reinterpret_cast<SineValue*>(buffer.data());

    std::cout << "Server received: ";
    for (int i = 0; i < count; ++i)
        std::cout << sinus[i] << ", ";
    std::cout << "from Client" << connectedClients[dataSender].index << "\n" << "\n";

    buffer.remove(0, count * sizeof(SineValue));
}

void Server::onDisconnected() {
    QTcpSocket* disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!disconnectedSocket) return;

    connectedClients.remove(disconnectedSocket);
    disconnectedSocket->deleteLater();
}

void Server::onErrorOccurred() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    qDebug() << "Unknown error";
    connectedClients.remove(socket);
    socket->deleteLater();
}

void Server::onStateChanged() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    switch (socket->state()) {
    case QAbstractSocket::HostLookupState:
        qDebug() << "Client" << connectedClients[socket].index << "is looking for host..." << "\n";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << "Client" << connectedClients[socket].index << "is connecting to server..." << "\n";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << "Client" << connectedClients[socket].index << "is connected" << "\n";
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << "Client" << connectedClients[socket].index << "is closing..." << "\n";
        break;
    case QAbstractSocket::UnconnectedState:
        qDebug() << "Client" << connectedClients[socket].index << "is unconnected" << "\n";
        break;
    default:
        break;
    }
}

void Server::sendRandBytesToClient(QTcpSocket* client){

    quint64 bytes = QRandomGenerator::global()->bounded(0,1000);
    QString valueType = "int";

    PackageType packageType(valueType, bytes);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << packageType;

    client->write(block);
}
