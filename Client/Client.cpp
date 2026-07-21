#include <QTcpSocket>
#include <QtGlobal>
#include <QIODevice>
#include "Client.h"
#include <QDataStream>
#include <QtMath>
#include <QThread>
#include <QTimer>
#include <QTimerEvent>

typedef int SineValue;

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

    m_sinus.reserve(sizeof(SineValue) * 1000);

    SineValue sineValueForQByteArray = 0;

    for (int i = 0; i < 1000; ++i) {
        sineValueForQByteArray = qSin(i * 2 * M_PI / 1000) * INT16_MAX;
        m_sinus.append(reinterpret_cast<char*>(&sineValueForQByteArray), sizeof(SineValue));
    }
}

Client::~Client() {}

void Client::connectedToServer() {

    if (m_timerIdForReconnect != 0) {
        killTimer(m_timerIdForReconnect);
        m_timerIdForReconnect = 0;
    }

    qDebug() << "Connected to server";
}

void Client::clientDisconnected() {

    m_timerForSend.stop();
    qDebug() << "Disconnected from server";

    if (m_timerIdForReconnect == 0) {
        m_timerIdForReconnect = startTimer(5000);
    }
}

void Client::read() {

    QDataStream rawBytes(m_socket->readAll());

    quint64 tempBytes;
    rawBytes >> tempBytes;
    m_countOfBytesForSendToServer = tempBytes;

    m_timerForSend.start(5000);
}

void Client::timeForSend() {

    QByteArray data = getPartOfSine();

    QByteArray data1;
    QByteArray data2;
    QByteArray data3;

    data1.append(data.constData(), data.size() / 3);
    data2.append(data.constData() + data1.size(), data.size() / 3);
    data3.append(data.constData() + data1.size() + data2.size(), data.size() - data1.size() - data2.size());

    qDebug() << this << "send: " << data1;
    m_socket->write(data1);
    m_socket->flush();

    qDebug() << this << "send: " << data2;
    m_socket->write(data2);
    m_socket->flush();

    qDebug() << this << "send: " << data3;
    m_socket->write(data3);
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

    QByteArray block;
    block.reserve(m_countOfBytesForSendToServer * sizeof(SineValue));

    int count = m_sinus.size() / sizeof(SineValue);

    if (m_countOfBytesForSendToServer + m_lastSinePositionInSinusArray < count)
        block.append(m_sinus.constData() + m_lastSinePositionInSinusArray * sizeof(SineValue),
                     m_countOfBytesForSendToServer * sizeof(SineValue));
    else {
        int partCountOfBytesForSendToServer = m_countOfBytesForSendToServer + m_lastSinePositionInSinusArray - count;
        block.append(m_sinus.constData() + m_lastSinePositionInSinusArray * sizeof(SineValue),
                     m_countOfBytesForSendToServer * sizeof(SineValue) - partCountOfBytesForSendToServer * sizeof(SineValue));
        block.append(m_sinus.constData(), partCountOfBytesForSendToServer * sizeof(SineValue));
    }

    m_lastSinePositionInSinusArray += m_countOfBytesForSendToServer;

    if (m_lastSinePositionInSinusArray >= count)
        m_lastSinePositionInSinusArray -= count;

    return block;
}
