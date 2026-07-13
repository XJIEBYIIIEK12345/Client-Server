#ifndef SERVER_H
#define SERVER_H
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QDebug>

class Server : public QTcpServer {
    Q_OBJECT

    QList<QTcpSocket*> connectedClientSockets;

public:
    explicit Server(QObject *parent = nullptr);

    bool startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void read();
    void disconnect();
};

#endif // SERVER_H
