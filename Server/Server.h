#ifndef SERVER_H
#define SERVER_H
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QDebug>

struct ClientData {
    quint32 index;
    QByteArray m_buffer;

    ClientData() : index(0) {}
    ClientData(quint32 _index) : index(_index) {}
};

class Server : public QTcpServer {
    Q_OBJECT

public:
    explicit Server(quint16 port, QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred();
    void onStateChanged();

private:
    void sendRandBytesToClient(QTcpSocket* client);

private:
    QMap<QTcpSocket*, ClientData> connectedClients;
};

#endif // SERVER_H
