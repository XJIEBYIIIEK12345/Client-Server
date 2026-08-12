#include "Server.h"
#include <QRandomGenerator64>
#include <QDataStream>
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

Server::Server(quint16 port, QString protocol, QObject *parent) : QTcpServer(parent) {

    if (!listen(QHostAddress::Any, port)) {
        qDebug() << "Server was not started\n";
    } else {
        qDebug() << "Server was started on port:" << port << "\n";
        m_serverProtocol = protocol;
        qDebug() << "Server is working with" << protocol << "data format";
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);

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

    int keepcnt = 3;
    int keepidle = 10;
    int keepintvl = 10;

    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
    setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
    setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));

    m_connectedClients[socket].m_clientProtocol = IProtocol::makeProtocol(m_serverProtocol);

    qDebug() << "Client" << m_connectedClients[socket].m_id << " was connected";
}

void Server::parseMessageFromClient() {

    QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
    if (!dataSender) return;

    //QByteArray messageFromClient = dataSender->readAll();
    m_connectedClients[dataSender].m_clientProtocol->m_buffer.append(dataSender->readAll());

    if (m_connectedClients[dataSender].m_clientProtocol->decodeData()) {

        qint32 count = m_connectedClients[dataSender].m_clientProtocol->m_package.count;
        QString type = m_connectedClients[dataSender].m_clientProtocol->m_package.type;
        QByteArray data = m_connectedClients[dataSender].m_clientProtocol->m_package.data;

        if (m_connectedClients[dataSender].m_clientProtocol->m_buffer.size() != 0)
            m_connectedClients[dataSender].m_clientProtocol->m_buffer.clear();

        QRandomGenerator randomGenerator;

        if (type == "connection" && data == "Hello") {

            PackageParserType packageParserType = PackageParserType(randomGenerator.bounded(0, int(PackageParserType::Count) - 1));

            QString valueType = packageParserTypeName(packageParserType);
            m_connectedClients[dataSender].m_parser = PackageParser::makeParser(packageParserType);

            quint32 bytes = randomGenerator.bounded(0, 1000);

            qDebug() << bytes << m_connectedClients[dataSender].m_id;

            //m_connectedClients[dataSender].m_clientProtocol->m_package.setPackageData(bytes, "sinRequest", valueType.toUtf8());
            m_connectedClients[dataSender].m_clientProtocol->m_package.setPackageData(bytes, "sinRequest", valueType.toUtf8());

            QByteArray messageFromServer = m_connectedClients[dataSender].m_clientProtocol->encodeData();

            dataSender->write(messageFromServer);
        }
        else if (type == "sinAnswer") {

            if (count <= data.size()) {

                QByteArray ok = "ok";

                m_connectedClients[dataSender].m_clientProtocol->m_package.setPackageData(ok.size(), "sinConfirmation", ok);
                QByteArray messageFromServer = m_connectedClients[dataSender].m_clientProtocol->encodeData();

                dataSender->write(messageFromServer);
                m_connectedClients[dataSender].m_parser->parseAndPrintPackage(data, m_connectedClients[dataSender].m_id);
            }
        }
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

