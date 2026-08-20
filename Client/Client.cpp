#include "Client.h"
#include "IProtocol.h"
#include "Package.h"
#include <QDataStream>
#include <QIODevice>
#include <QTcpSocket>
#include <QTimer>
#include <QTimerEvent>
#include <netinet/in.h>
#include <netinet/tcp.h>

Client::Client(QString address, quint16 port, ProtocolDataType protocol,
               QObject* parent)
  : QObject(parent)
{
  m_address = address;
  m_port = port;
  m_processor = new MessageProcessorForClient(protocol);

  qDebug() << "Client is waiting for server on address:" << address
           << ", port:" << port;
  qDebug() << "Client is working with" << IProtocol::toString(protocol)
           << "data format";
}

Client::~Client() {}

void Client::connect()
{
  m_socket = new QTcpSocket(this);

  int keepcnt = 3;
  int keepidle = 10;
  int keepintvl = 10;

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
                      &Client::startReconnectTimer);
  QTcpSocket::connect(m_socket, &QTcpSocket::connected, this,
                      &Client::initPackageSending);
  QTcpSocket::connect(m_socket, &QTcpSocket::errorOccurred, this,
                      &Client::closeSocket);
  QObject::connect(m_processor, &MessageProcessorForClient::readyToSend, this,
                   &Client::writeToServer);

  m_timerIdForConnect = startTimer(1);
}

void Client::initPackageSending()
{
  if (m_timerIdForConnect != 0)
  {
    killTimer(m_timerIdForConnect);
    m_timerIdForConnect = 0;
  }

  m_processor->clientConnectedToServer();
}

void Client::startReconnectTimer()
{
  m_processor->stopSending();

  qDebug() << "Disconnected from server";

  if (m_timerIdForConnect == 0)
  {
    m_timerIdForConnect = startTimer(5000);
  }
}

void Client::readDataFromServer()
{
  QByteArray message = m_socket->readAll();

  m_processor->parseMessage(message);
}

void Client::closeSocket()
{
  qDebug() << "Error:" << m_socket->errorString();
  m_socket->close();
}

void Client::sendPackageToServer()
{
  m_processor->stopSending();

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
  qDebug() << "Connecting...";
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
