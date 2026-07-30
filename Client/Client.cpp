#include <QTcpSocket>
#include <QIODevice>
#include "Client.h"
#include <QDataStream>
#include <QTimer>
#include <QTimerEvent>
#include "PackageTypeForClient.h"
#include "SineGenerator.h"
#include <netinet/tcp.h>
#include <netinet/in.h>

Client::Client(QString address, quint16 port, QObject *parent) : QObject(parent) {

    m_address = address;
    m_port = port;
}

Client::~Client() {}

void Client::init() {

    m_socket = new QTcpSocket(this);
    QTcpSocket::connect(m_socket, &QTcpSocket::readyRead, this, &Client::readDataFromServer);
    QTcpSocket::connect(m_socket, &QTcpSocket::disconnected, this, &Client::startReconnectTimer);
    QTcpSocket::connect(m_socket, &QTcpSocket::connected, this, &Client::sendHelloToServer);

    m_timerIdForReconnect = startTimer(5000);
}

void Client::sendHelloToServer() {

    if (m_timerIdForReconnect != 0) {
        killTimer(m_timerIdForReconnect);
        m_timerIdForReconnect = 0;
    }

    int keepcnt = 3;
    int keepidle = 10;
    int keepintvl = 10;

    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    setsockopt(m_socket->socketDescriptor(), IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
    setsockopt(m_socket->socketDescriptor(), IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
    setsockopt(m_socket->socketDescriptor(), IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));

    qDebug() << "Connected to server";

    QByteArray message = "Hello";
    m_socket->write(message);
}

void Client::startReconnectTimer() {

    if (m_timerIdForSend != 0) {
        killTimer(m_timerIdForSend);
        m_timerIdForSend = 0;
    }

    qDebug() << "Disconnected from server";

    if (m_generator != nullptr) {
        delete m_generator;
        m_generator = nullptr;
    }

    if (m_timerIdForReconnect == 0) {
        m_timerIdForReconnect = startTimer(5000);
    }
}

void Client::readDataFromServer() {

    QByteArray message = m_socket->readAll();

    if (message == "ok") {

        if (m_timerIdForSend == 0) {
            m_timerIdForSend = startTimer(2500);
        }
    }
    else {

        QDataStream data(message);

        PackageTypeToClient package;
        data >> package;

        m_countOfBytes = package.bytes;
        m_generator = SineGenerator::makeGenerator(package.valueType);

        if (m_timerIdForSend == 0) {
            m_timerIdForSend = startTimer(2500);
        }
    }
}

void Client::sendPackageToServer() {

    if (m_timerIdForSend != 0) {
        killTimer(m_timerIdForSend);
        m_timerIdForSend = 0;
    }

    QByteArray data = m_generator->generateSineForType(m_countOfBytes);

    qDebug() << this << "send: " << data;

    m_socket->write(data);

    if (!m_socket->waitForReadyRead(10000)) {
        m_socket->close();
        if (m_timerIdForReconnect == 0) {
            m_timerIdForReconnect = startTimer(5000);
        }
    }
}

void Client::connectToServer() {

    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Connecting...";
        m_socket->connectToHost(QHostAddress(m_address), m_port, QTcpSocket::ReadWrite);
    }
}

void Client::timerEvent(QTimerEvent *event) {

    if (event->timerId() == m_timerIdForReconnect) {
        connectToServer();
    } else if (event->timerId() == m_timerIdForSend) {
        sendPackageToServer();
    } else {
        QObject::timerEvent(event);
    }
}
