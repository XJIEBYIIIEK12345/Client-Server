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

    ClientData() : m_id(0) {}
    ClientData(quint32 _id) : m_id(_id) {}
    ~ClientData() {
        if (m_parser != nullptr) {
            delete m_parser;
            m_parser = nullptr;
        }
    }
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
