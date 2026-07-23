#include "Server.h"
#include <QRandomGenerator64>
#include <QDataStream>
#include <iostream>
#include "PackageTypeForServer.h"
#include "PackageParser.h"
#include "PackageParserForInt16.h"
#include "PackageParserForInt32.h"
#include "PackageParserForInt64.h"

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

    qDebug() << "Client" << connectedClients[socket].m_id << " was connected";
}

void Server::onReadyRead() {

    QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
    if (!dataSender) return;

    QByteArray message = dataSender->readAll();

    if (message == "Hello") {

        QString valueType;
        switch (QRandomGenerator::global()->bounded(1, 3)) {
        case 1:
            connectedClients[dataSender].m_parser = new PackageParserForInt16();
            valueType = "qint16";
            break;
        case 2:
            connectedClients[dataSender].m_parser = new PackageParserForInt32();
            valueType = "qint32";
            break;
        case 3:
            connectedClients[dataSender].m_parser = new PackageParserForInt64();
            valueType = "qint64";
            break;
        default:
            connectedClients[dataSender].m_parser = new PackageParserForInt32();
            valueType = "qint32";
            break;
        }

        quint64 bytes = QRandomGenerator::global()->bounded(0,1000);

        PackageTypeToClient package(valueType, bytes);

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        out << package;

        dataSender->write(block);
    }
    else {
        connectedClients[dataSender].m_parser->parsePackage(message);
        std::cout << "from Client" << connectedClients[dataSender].m_id << "\n" << "\n";
    }
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
        qDebug() << "Client" << connectedClients[socket].m_id << "is looking for host..." << "\n";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << "Client" << connectedClients[socket].m_id << "is connecting to server..." << "\n";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << "Client" << connectedClients[socket].m_id << "is connected" << "\n";
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << "Client" << connectedClients[socket].m_id << "is closing..." << "\n";
        break;
    case QAbstractSocket::UnconnectedState:
        qDebug() << "Client" << connectedClients[socket].m_id << "is unconnected" << "\n";
        break;
    default:
        break;
    }
}

