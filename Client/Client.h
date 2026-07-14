#ifndef CLIENT_H
#define CLIENT_H
#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

class Client : public QObject {
    Q_OBJECT
    QTcpSocket* socket = nullptr;
    QTimer* timer = nullptr;

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void connectToServer(const QHostAddress &serverAddress, quint16 port);

public slots:
    void connectedToServer();
    void clientDisconnected();
    void read();

private:
    QByteArray generateArr(quint64 bytes, quint64 valFrom);
};

#endif // CLIENT_H
