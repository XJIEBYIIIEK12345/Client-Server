#ifndef SERVER_H
#define SERVER_H
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDebug>

class Server : public QTcpServer {
    Q_OBJECT

    QVector<QTcpSocket*> connectedClients;

public:
    explicit Server(QObject *parent = nullptr);

    bool startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void read();
    void disconnect();
    void writeToClient(QTcpSocket* client);
};

#endif // SERVER_H
