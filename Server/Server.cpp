#include "Server.h"
#include <QRandomGenerator64>
#include <QDataStream>
#include "PackageTypeForServer.h"
#include "PackageParser.h"
#include <netinet/tcp.h>
#include <netinet/in.h>


ClientData::ClientData() : m_id(0) {}

ClientData::ClientData(quint32 _id) : m_id(_id) {}

ClientData::~ClientData() {
    if (m_parser != nullptr) {
        delete m_parser;
        m_parser = nullptr;
    }
}

Server::Server(quint16 port, QObject *parent) : QTcpServer(parent) {

    if (!listen(QHostAddress::Any, port)) {
        qDebug() << "Server was not started\n";
    } else {
        qDebug() << "Server was started on port:" << port << "\n";
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);

    int keepcnt = 5;
    int keepidle = 30;
    int keepintvl = 120;

    setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
    setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
    setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));

    if (socket->setSocketDescriptor(socketDescriptor)) {
        connect(socket, &QTcpSocket::readyRead, this, &Server::parseMessageFromClient);
        connect(socket, &QTcpSocket::disconnected, this, &Server::cleanClientData);
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
        connect(socket, &QTcpSocket::errorOccurred, this, &Server::closeClientSocket);
        connect(socket, &QTcpSocket::stateChanged, this, &Server::logClientState);
        m_connectedClients.insert(socket, ClientData(m_connectedClients.size() + 1));
    } else {
        delete socket;
    }

    qDebug() << "Client" << m_connectedClients[socket].m_id << " was connected";
}

void Server::parseMessageFromClient() {

    QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
    if (!dataSender) return;

    QByteArray message = dataSender->readAll();

    QRandomGenerator randomGenerator;

    if (message == "Hello") {

        PackageParserType packageParserType = PackageParserType(randomGenerator.bounded(0, int(PackageParserType::Count) - 1));

        QString valueType = packageParserTypeName(packageParserType);
        m_connectedClients[dataSender].m_parser = PackageParser::makeParser(packageParserType);

        quint32 bytes = 100000;//randomGenerator.bounded(0,1000);

        PackageTypeToClient package(valueType, bytes);

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        out << package;

        dataSender->write(block);
    }
    else {
        m_connectedClients[dataSender].m_parser->parseAndPrintPackage(message, m_connectedClients[dataSender].m_id);
    }
}

void Server::cleanClientData() {
    QTcpSocket* disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!disconnectedSocket) return;

    m_connectedClients.remove(disconnectedSocket);
}

void Server::closeClientSocket() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    qDebug() << "Unknown error";
    socket->close();
}

void Server::logClientState() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    switch (socket->state()) {
    case QAbstractSocket::HostLookupState:
        qDebug() << "Client" << m_connectedClients[socket].m_id << "is looking for host..." << "\n";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << "Client" << m_connectedClients[socket].m_id << "is connecting to server..." << "\n";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << "Client" << m_connectedClients[socket].m_id << "is connected" << "\n";
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << "Client" << m_connectedClients[socket].m_id << "is closing..." << "\n";
        break;
    case QAbstractSocket::UnconnectedState:
        qDebug() << "Client" << m_connectedClients[socket].m_id << "is unconnected" << "\n";
        break;
    default:
        break;
    }
}

