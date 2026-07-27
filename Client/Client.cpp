#include <QTcpSocket>
#include <QIODevice>
#include "Client.h"
#include <QDataStream>
#include <QTimer>
#include <QTimerEvent>
#include "PackageTypeForClient.h"
#include "SineGenerator.h"

Client::Client(QString address, quint16 port, QObject *parent) : QObject(parent) {

    m_address = address;
    m_port = port;
    m_socket = new QTcpSocket(this);
    QTimer::connect(&m_timerForSend, &QTimer::timeout, this, &Client::sendPackageToServer);
    QTcpSocket::connect(m_socket, &QTcpSocket::readyRead, this, &Client::readDataFromServer);
    QTcpSocket::connect(m_socket, &QTcpSocket::disconnected, this, &Client::startReconnectTimer);
    QTcpSocket::connect(m_socket, &QTcpSocket::connected, this, &Client::sendHelloToServer);

    m_socket->connectToHost(QHostAddress(m_address), m_port, QTcpSocket::ReadWrite);

    if (!m_socket->waitForConnected(1000)) {

        qDebug() << "Failed to connect to server. Reconnecting...";

        if (m_timerIdForReconnect == 0) {
            m_timerIdForReconnect = startTimer(5000);
        }
    }
}

Client::~Client() {}

void Client::sendHelloToServer() {

    if (m_timerIdForReconnect != 0) {
        killTimer(m_timerIdForReconnect);
        m_timerIdForReconnect = 0;
    }

    qDebug() << "Connected to server";

    QByteArray message;
    message = "Hello";

    m_socket->write(message);
}

void Client::startReconnectTimer() {

    m_timerForSend.stop();
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

    QDataStream data(m_socket);
    PackageTypeToClient package;

    data >> package;

    m_generator = SineGenerator::makeGenerator(package.valueType, package.bytes);

    m_timerForSend.start(5000);
}

void Client::sendPackageToServer() {

    QByteArray data = m_generator->generateSineForType();
    qDebug() << this << "send: " << data;

    m_socket->write(data);
    m_socket->flush();
}

void Client::timerEvent(QTimerEvent *event) {

    if (event->timerId() == m_timerIdForReconnect) {
        qDebug() << "Reconnecting...";
        m_socket->connectToHost(QHostAddress(m_address), m_port, QTcpSocket::ReadWrite);
    } else {
        QObject::timerEvent(event);
    }
}
