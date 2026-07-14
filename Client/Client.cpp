#include <QTcpSocket>
#include <QtGlobal>
#include <QIODevice>
#include "Client.h"
#include <QDataStream>
#include <QtMath>
#include <QThread>
#include <QTimer>

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
    socket->flush();
}

void Client::connectedToServer() {

    qDebug() << "Connected to server";
}

void Client::clientDisconnected() {

    timer->stop();
    qDebug() << "Disconnected from server";
}

void Client::read() {

    if (timer != nullptr)
        delete timer;

    timer = new QTimer(this);

    QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
    QDataStream rawBytes(dataSender->readAll());

    quint64 bytes;
    quint64 valFrom = 0;
    rawBytes >> bytes;

    QTimer::connect(timer, &QTimer::timeout, this, [=]() mutable {
        QByteArray data = generateArr(bytes, valFrom);

        qDebug() << this << "send: " << data;

        valFrom += bytes;

        socket->write(data);
    });

    timer->start(5000);
}

QByteArray Client::generateArr(quint64 bytes, quint64 valFrom) {

    QByteArray block(bytes, Qt::Uninitialized);

    QDataStream stream(&block, QIODevice::WriteOnly);

    stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
    stream.setByteOrder(QDataStream::BigEndian);

    for (int i = valFrom; i < int(bytes) + (int)valFrom; ++i) {

        double progress = static_cast<double>(i) / 1000;
        double rad = progress * 2.0 * M_PI;
        double sineVal = qSin(rad);

        stream << sineVal;
    }

    return block;
}
