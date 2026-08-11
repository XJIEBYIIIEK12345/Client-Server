#ifndef SERVER_H
#define SERVER_H
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "PackageParser.h"

struct ClientData {
    quint32 m_id;
    PackageParser* m_parser = nullptr;
    QByteArray m_jsonBuffer;

    ClientData();
    ClientData(quint32 _id);
    ~ClientData();
};

class Server : public QTcpServer {
    Q_OBJECT

public:
    explicit Server(quint16 port, QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void parseMessageFromClient();
    void cleanClientData();
    void closeClientSocket();
    void logClientState();

private:
    QMap<QTcpSocket*, ClientData> m_connectedClients;
};

#endif // SERVER_H
