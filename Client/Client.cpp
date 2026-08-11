#include <QTcpSocket>
#include <QIODevice>
#include "Client.h"
#include <QDataStream>
#include <QTimer>
#include <QTimerEvent>
#include "PackageType.h"
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

    QByteArray data = "Hello";

    QJsonObject jsonObj = createJsonObject(data.size(), "connection", data);
    QByteArray message = jsonObjToByteArray(jsonObj);

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

    QJsonObject jsonObjFromServer = byteArrayToJsonObj(message);

    qint32 count = jsonObjFromServer["count"].toInt();
    QString type = jsonObjFromServer["type"].toString();
    QByteArray data = QByteArray::fromBase64(jsonObjFromServer["data"].toString().toUtf8());

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

void Client::sendPackageToServer() {

    if (m_timerIdForSend != 0) {
        killTimer(m_timerIdForSend);
        m_timerIdForSend = 0;
    }

    QByteArray data = m_generator->generateSineForType(m_countOfBytes);

    QJsonObject jsonObjFromClient = createJsonObject(data.size(), "sinAnswer", data);
    QByteArray message = jsonObjToByteArray(jsonObjFromClient);

    qDebug() << this << "send: " << data;

    m_socket->write(message);

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
