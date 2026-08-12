#ifndef SERVER_H
#define SERVER_H
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "PackageParser.h"
#include "IProtocol.h"

struct ClientData {
    quint32 m_id;
    PackageParser* m_parser = nullptr;
    IProtocol* m_clientProtocol = nullptr;

    ClientData();
    ClientData(quint32 _id);
    ~ClientData();
};

class Server : public QTcpServer {
    Q_OBJECT

public:
    explicit Server(quint16 port, QString protocol, QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void parseMessageFromClient();
    void cleanClientData();
    void closeClientSocket();
    void logClientState();

private:
    QMap<QTcpSocket*, ClientData> m_connectedClients;
    QString m_serverProtocol;
};

#endif // SERVER_H
