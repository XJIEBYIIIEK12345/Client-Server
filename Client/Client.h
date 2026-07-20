#ifndef CLIENT_H
#define CLIENT_H
#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

private slots:
    void connectedToServer();
    void clientDisconnected();
    void read();
    void timeForSend();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    QByteArray getPartOfSine();

private:
    quint16 m_port = 6789;
    QTcpSocket* m_socket = nullptr;
    QTimer m_timerForSend;
    int m_timerIdForReconnect = 0;
    int m_countOfBytesForSendToServer = 0;
    int m_lastSinePositionInSinusArray = 0;
    QByteArray m_sinus;
};

#endif // CLIENT_H
