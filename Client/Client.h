#ifndef CLIENT_H
#define CLIENT_H
#pragma once
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include "SineGenerator.h"

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QString address, quint16 port, QObject *parent = nullptr);
    ~Client();

private slots:
    void connectedToServer();
    void clientDisconnected();
    void read();
    void timeForSend();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    quint16 m_port = 6789;
    QString m_address = "127.0.0.1";
    QTcpSocket* m_socket = nullptr;
    QTimer m_timerForSend;
    int m_timerIdForReconnect = 0;
    SineGenerator* m_generator = nullptr;
};

#endif // CLIENT_H
