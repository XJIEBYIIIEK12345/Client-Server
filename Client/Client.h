#ifndef CLIENT_H
#define CLIENT_H
#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class Client : public QObject {
    Q_OBJECT
    QTcpSocket* socket;
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void connectToServer(const QHostAddress &serverAddress, quint16 port);

public slots:
    void connectedToServer();
    void clientDisconnected();
    void read();
};

#endif // CLIENT_H
