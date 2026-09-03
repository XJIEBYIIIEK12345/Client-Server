#ifndef SERVER_H
#define SERVER_H
#pragma once

#include "IProtocol.h"
#include <QTcpServer>

class Server : public QTcpServer
{
  Q_OBJECT

public:
  explicit Server(quint16 port, ProtocolDataType protocol, log4cplus::Logger logger,
                  QObject* parent = nullptr);

signals:
  void clientStartConnecting(quintptr socketDescriptor);

protected:
  void incomingConnection(qintptr socketDescriptor) override;

private:
  ProtocolDataType m_serverProtocol;
  log4cplus::Logger m_logger;
};

#endif // SERVER_H
