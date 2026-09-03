#include "WorkerThread.h"
#include <netinet/in.h>
#include <netinet/tcp.h>

WorkerThread::WorkerThread(log4cplus::Logger logger, quintptr socketDescriptor)
{
  m_logger = logger;
  m_socketDescriptor = socketDescriptor;
}

void WorkerThread::writeToReceiver(QByteArray message) { m_socket->write(message); }

quintptr WorkerThread::getSocketDescriptor() { return m_socketDescriptor; }

void WorkerThread::process()
{
  m_socket = new QTcpSocket();

  if (m_socket->setSocketDescriptor(m_socketDescriptor))
  {
    QTcpSocket::connect(m_socket, &QTcpSocket::readyRead, this,
                        &WorkerThread::readDataFromSender);
    QTcpSocket::connect(m_socket, &QTcpSocket::disconnected, this,
                        &WorkerThread::stop);
    QTcpSocket::connect(m_socket, &QTcpSocket::errorOccurred, this,
                        &WorkerThread::logErrorAndCloseSocket);
    QTcpSocket::connect(m_socket, &QTcpSocket::stateChanged, this,
                        &WorkerThread::logClientState);
  }
  else
  {
    LOG4CPLUS_ERROR(m_logger,
                    "System error: " << m_socket->errorString().toStdString());
    delete m_socket;
    return;
  }

  int keepcnt = 3;
  int keepidle = 5;
  int keepintvl = 5;

  m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

  setsockopt(m_socketDescriptor, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
  setsockopt(m_socketDescriptor, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
  setsockopt(m_socketDescriptor, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl,
             sizeof(int));
}

void WorkerThread::stop()
{
  if (!m_socket)
    return;

  m_socket->close();
  m_socket->deleteLater();

  LOG4CPLUS_TRACE(m_logger,
                  "The worker with id: " << m_socketDescriptor << " was stopped");

  m_socket = nullptr;
  m_socketDescriptor = 0;
  emit finished();
}

void WorkerThread::readDataFromSender()
{
  if (!m_socket)
    return;
  emit bytesReceived(m_socket->readAll(), m_socketDescriptor);
}

void WorkerThread::logErrorAndCloseSocket()
{
  if (!m_socket)
    return;
  LOG4CPLUS_ERROR(m_logger, m_socket->errorString().toStdString());
  m_socket->close();
}

void WorkerThread::logClientState()
{
  if (!m_socket)
    return;
  switch (m_socket->state())
  {
  case QAbstractSocket::HostLookupState:
    LOG4CPLUS_INFO(m_logger, "Client with id: " << m_socket->socketDescriptor()
                                                << " is looking for host...");
    break;
  case QAbstractSocket::ConnectingState:
    LOG4CPLUS_INFO(m_logger, "Client with id: " << m_socket->socketDescriptor()
                                                << " is connecting to server...");
    break;
  case QAbstractSocket::ConnectedState:
    LOG4CPLUS_INFO(m_logger, "Client with id: " << m_socket->socketDescriptor()
                                                << " is connected");
    break;
  case QAbstractSocket::ClosingState:
    LOG4CPLUS_INFO(m_logger, "Client with id: " << m_socket->socketDescriptor()
                                                << " is closing...");
    break;
  case QAbstractSocket::UnconnectedState:
    LOG4CPLUS_INFO(m_logger, "Client with id: " << m_socket->socketDescriptor()
                                                << " is unconnected");
    break;
  default:
    break;
  }
}
