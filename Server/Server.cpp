#include "Server.h"

Server::Server(quint16 port, ProtocolDataType protocol, log4cplus::Logger logger,
               QObject* parent)
  : QTcpServer(parent)
{
  m_logger = logger;

  if (!listen(QHostAddress::Any, port))
  {
    LOG4CPLUS_ERROR(m_logger, "Server was not started\n");
  }
  else
  {
    LOG4CPLUS_INFO(m_logger, "Server was started on port: " << port);
    m_serverProtocol = protocol;
    LOG4CPLUS_INFO(m_logger, "Server is working with "
                                 << IProtocol::toString(protocol).toStdString()
                                 << " data format");
  }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
  emit clientStartConnecting(socketDescriptor);
  LOG4CPLUS_INFO(m_logger,
                 "Client with id " << socketDescriptor << " was connected");
}
