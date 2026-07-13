#include "Server.h"

Server::Server(QObject *parent) : QTcpServer(parent) {
}

bool Server::startServer(quint16 port) {
    if (!listen(QHostAddress::Any, port)) {
        qDebug() << "Server was not started\n";
        return false;
    } else {
        qDebug() << "Server was started on port:" << port << "\n";
        return true;
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);

    if (socket->setSocketDescriptor(socketDescriptor)) {
        connect(socket, &QTcpSocket::readyRead, this, &Server::read);
        connect(socket, &QTcpSocket::disconnected, this, &Server::disconnect);
        connectedClientSockets.append(socket);
    } else {
        delete socket;
    }
}

void Server::read() {
    QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
    if (!dataSender) return;

    QByteArray data = dataSender->readAll();
    QString stringData = QString::fromUtf8(data);

    if (stringData.trimmed() == "echo") {
        dataSender->write("ok");
        qDebug() << "Server says: ok" << "\n";
    } else {
        dataSender->write("not ok");
        qDebug() << "Server says: not ok" << "\n";
    }
}

void Server::disconnect() {
    QTcpSocket* disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!disconnectedSocket) return;

    connectedClientSockets.removeOne(disconnectedSocket);
    disconnectedSocket->deleteLater();
}
