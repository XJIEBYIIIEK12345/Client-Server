#include "Server.h"

Server::Server(quint16 port, ProtocolDataType protocol, QObject* parent)
  : QTcpServer(parent)
{
  if (!listen(QHostAddress::Any, port))
  {
    qDebug() << "Server was not started\n";
  }
  else
  {
    qDebug() << "Server was started on port:" << port;
    m_serverProtocol = protocol;
    qDebug() << "Server is working with" << IProtocol::toString(protocol)
             << "data format";
  }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
  emit clientStartConnecting(socketDescriptor);
  qDebug() << "Client with id" << socketDescriptor << "was connected";
}
