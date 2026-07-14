#include "Server.h"
#include <QRandomGenerator64>
#include <QDataStream>

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
        connectedClients.append(socket);
    } else {
        delete socket;
    }
    this->writeToClient(socket);
}

void Server::read() {
    QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
    if (!dataSender) return;

    QByteArray data = dataSender->readAll();

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);

    QList<double> sinus;
    sinus.reserve(data.size());

    while (!stream.atEnd()) {
        double val;
        stream >> val;
        sinus.append(val);
    }

    qDebug() << "Server received: " << sinus << "from" << dataSender;
}

void Server::writeToClient(QTcpSocket* client){

    quint64 bytes = QRandomGenerator::global()->bounded(0,1000);

    QDataStream out(client);

    out << bytes;
}

void Server::disconnect() {
    QTcpSocket* disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!disconnectedSocket) return;

    connectedClients.removeOne(disconnectedSocket);
    disconnectedSocket->deleteLater();
}
