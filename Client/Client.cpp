#include "Client.h"
#include "IProtocol.h"
#include <QTcpSocket>
#include <QTimerEvent>
#include <netinet/in.h>
#include <netinet/tcp.h>

Client::Client(QString address, quint16 port, ProtocolDataType protocol,
               log4cplus::Logger logger, QObject* parent)
  : QObject(parent)
{
  m_address = address;
  m_port = port;
  m_logger = logger;

  LOG4CPLUS_INFO(m_logger, "Client is waiting for server on address: "
                               << address.toStdString() << ", port: " << port);

  LOG4CPLUS_INFO(m_logger, "Client is working with "
                               << IProtocol::toString(protocol).toStdString()
                               << " data format");
}

Client::~Client()
{
  if (m_socket != nullptr)
  {
    m_socket->deleteLater();
    m_socket = nullptr;
  }
}

void Client::connect()
{
  m_socket = new QTcpSocket(this);

  int keepcnt = 3;
  int keepidle = 5;
  int keepintvl = 5;

  m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

  setsockopt(m_socket->socketDescriptor(), IPPROTO_TCP, TCP_KEEPCNT, &keepcnt,
             sizeof(int));
  setsockopt(m_socket->socketDescriptor(), IPPROTO_TCP, TCP_KEEPIDLE, &keepidle,
             sizeof(int));
  setsockopt(m_socket->socketDescriptor(), IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl,
             sizeof(int));

  QTcpSocket::connect(m_socket, &QTcpSocket::readyRead, this,
                      &Client::readDataFromServer);
  QTcpSocket::connect(m_socket, &QTcpSocket::disconnected, this,
                      &Client::startReconnecting);
  QTcpSocket::connect(m_socket, &QTcpSocket::connected, this,
                      &Client::stopReconnecting);
  QTcpSocket::connect(m_socket, &QTcpSocket::errorOccurred, this, &Client::logError);

  m_timerIdForConnect = startTimer(1);
}

void Client::closeAll()
{
  emit notReadyForSend();
  m_socket->close();

  if (m_socket != nullptr)
  {
    m_socket->deleteLater();
    m_socket = nullptr;
  }
}

void Client::stopReconnecting()
{
  if (m_timerIdForConnect != 0)
  {
    killTimer(m_timerIdForConnect);
    m_timerIdForConnect = 0;
  }

  emit readyForSend();
}

void Client::startReconnecting()
{
  emit notReadyForSend();

  LOG4CPLUS_INFO(m_logger, "Disconnected from server");

  if (m_timerIdForConnect == 0)
  {
    m_timerIdForConnect = startTimer(5000);
  }
}

void Client::readDataFromServer()
{
  QByteArray message = m_socket->readAll();

  emit bytesReceived(message);
}

void Client::logError()
{
  LOG4CPLUS_ERROR(m_logger, m_socket->errorString().toStdString());
  m_socket->close();
}

void Client::closeSocket()
{
  if (!m_socket->waitForReadyRead(10000))
  {
    m_socket->close();

    if (m_timerIdForConnect == 0)
    {
      m_timerIdForConnect = startTimer(5000);
    }
  }
}

void Client::connectToServer()
{
  LOG4CPLUS_INFO(m_logger, "Connecting...");
  m_socket->connectToHost(QHostAddress(m_address), m_port, QTcpSocket::ReadWrite);
}

void Client::writeToServer(QByteArray message) { m_socket->write(message); }

void Client::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == m_timerIdForConnect)
  {
    if ((m_socket->state() != QAbstractSocket::ConnectingState) &&
        (m_socket->state() != QAbstractSocket::ConnectedState))
    {
      connectToServer();
    }
  }
  else
  {
    QObject::timerEvent(event);
  }
}
