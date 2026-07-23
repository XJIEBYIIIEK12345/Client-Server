#include <QTcpSocket>
#include <QIODevice>
#include "Client.h"
#include <QDataStream>
#include <QTimer>
#include <QTimerEvent>
#include "PackageType.h"
#include "SineGenerator.h"
#include "SineGeneratorForInt16.h"
#include "SineGeneratorForInt32.h"
#include "SineGeneratorForInt64.h"

Client::Client(QString address, quint16 port, QObject *parent) : QObject(parent) {

    m_address = address;
    m_port = port;
    m_socket = new QTcpSocket(this);
    QTimer::connect(&m_timerForSend, &QTimer::timeout, this, &Client::timeForSend);
    QTcpSocket::connect(m_socket, &QTcpSocket::readyRead, this, &Client::read);
    QTcpSocket::connect(m_socket, &QTcpSocket::disconnected, this, &Client::clientDisconnected);
    QTcpSocket::connect(m_socket, &QTcpSocket::connected, this, &Client::connectedToServer);

    m_socket->connectToHost(QHostAddress(m_address), m_port, QTcpSocket::ReadWrite);

    if (!m_socket->waitForConnected(1000)) {

        qDebug() << "Failed to connect to server. Reconnecting...";

        if (m_timerIdForReconnect == 0) {
            m_timerIdForReconnect = startTimer(5000);
        }
    }
}

Client::~Client() {}

void Client::connectedToServer() {

    if (m_timerIdForReconnect != 0) {
        killTimer(m_timerIdForReconnect);
        m_timerIdForReconnect = 0;
    }

    qDebug() << "Connected to server";

    QByteArray message;
    message = "Hello";

    m_socket->write(message);

    m_lastSinePositionInSinusArray = 0;
}

void Client::clientDisconnected() {

    m_timerForSend.stop();
    qDebug() << "Disconnected from server";

    if (m_timerIdForReconnect == 0) {
        m_timerIdForReconnect = startTimer(5000);
    }
}

void Client::read() {

    QDataStream data(m_socket);

    PackageTypeToClient package;

    data >> package;

    m_countOfBytesForSendToServer = package.bytes;
    QString typeForPackageToSend = package.valueType;

    if (typeForPackageToSend == "qint16") {
        m_generator = new SineGeneratorForInt16();
    }
    else if (typeForPackageToSend == "qint32") {
        m_generator = new SineGeneratorForInt32();
    }
    else if (typeForPackageToSend == "qint64") {
        m_generator = new SineGeneratorForInt64();
    }
    else {
        m_generator = new SineGeneratorForInt32();
    }

    m_timerForSend.start(5000);
}

void Client::timeForSend() {

    QByteArray data;
    data = getPartOfSine();
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

QByteArray Client::getPartOfSine() {

    QByteArray block = m_generator->generateSineForType(m_countOfBytesForSendToServer, m_lastSinePositionInSinusArray);

    m_lastSinePositionInSinusArray += m_countOfBytesForSendToServer;

    if (m_lastSinePositionInSinusArray >= 1000)
        m_lastSinePositionInSinusArray -= 1000;

    return block;
}
