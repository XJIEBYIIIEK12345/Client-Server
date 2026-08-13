#include <QTcpSocket>
#include <QIODevice>
#include "Client.h"
#include <QDataStream>
#include <QTimer>
#include <QTimerEvent>
#include "SineGenerator.h"
#include <netinet/tcp.h>
#include <netinet/in.h>
#include "IProtocol.h"

Client::Client(QString address, quint16 port, QString protocol, QObject *parent) : QObject(parent) {

    m_address = address;
    m_port = port;
    m_protocol = IProtocol::makeProtocol(protocol);

    qDebug() << "Client is waiting for server on address:" << address << ", port:" << port;
    qDebug() << "Client is working with" << protocol << "data format";
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

    QByteArray data = "Hello";
    Package* pack = new Package(data.size(), "connection", data);

    QByteArray message = m_protocol->encodeData(pack);

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
    m_protocol->m_buffer.append(message);

    Package* pack = m_protocol->decodeData();

    if (pack != nullptr) {

        qint32 count = pack->m_count;
        QString type = pack->m_type;
        QByteArray data = pack->m_data;

        if (m_protocol->m_buffer.size() != 0)
            m_protocol->m_buffer.clear();
        if (pack != nullptr) {
            delete pack;
            pack = nullptr;
        }

        if (type == "sinRequest") {

            m_countOfBytes = count;
            m_generator = SineGenerator::makeGenerator(QString(data));

            if (m_timerIdForSend == 0) {
                m_timerIdForSend = startTimer(2500);
            }
        } else if (type == "sinConfirmation" && data == "ok") {

            if (m_timerIdForSend == 0) {
                m_timerIdForSend = startTimer(2500);
            }
        }
    }
}

void Client::sendPackageToServer() {

    if (m_timerIdForSend != 0) {
        killTimer(m_timerIdForSend);
        m_timerIdForSend = 0;
    }
    QByteArray data = m_generator->generateSineForType(m_countOfBytes);

    Package* pack = new Package(data.size(), "sinAnswer", data);
    QByteArray message = m_protocol->encodeData(pack);

    qDebug() << this << "send: " << data;

    m_socket->write(message);

    if (pack != nullptr) {
        delete pack;
        pack = nullptr;
    }

    if (!m_socket->waitForReadyRead(10000)) {
        m_socket->close();
        if (m_timerIdForReconnect == 0) {
            m_timerIdForReconnect = startTimer(5000);
        }
    }
}

void Client::connectToServer() {

    if ((m_socket->state() != QAbstractSocket::ConnectedState) && (m_socket->state() != QAbstractSocket::ConnectingState)) {
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
