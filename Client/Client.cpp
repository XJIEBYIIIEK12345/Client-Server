#include "Client.h"
#include "IProtocol.h"
#include "Package.h"
#include "PackageForDataToGenerate.h"
#include "PackageForGeneratedData.h"
#include "PackageForSignal.h"
#include "SineGenerator.h"
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
  m_protocol = IProtocol::makeProtocol(protocol);

  qDebug() << "Client is waiting for server on address:" << address
           << ", port:" << port;
  qDebug() << "Client is working with" << IProtocol::toString(protocol)
           << "data format";
}

Client::~Client() {}

void Client::connect()
{
  m_socket = new QTcpSocket(this);
  QTcpSocket::connect(m_socket, &QTcpSocket::readyRead, this,
                      &Client::readDataFromServer);
  QTcpSocket::connect(m_socket, &QTcpSocket::disconnected, this,
                      &Client::startReconnectTimer);
  QTcpSocket::connect(m_socket, &QTcpSocket::connected, this,
                      &Client::sendHelloToServer);
  QTcpSocket::connect(m_socket, &QTcpSocket::errorOccurred, this,
                      &Client::closeSocket);

  m_timerIdForReconnect = startTimer(1);
}

void Client::sendHelloToServer()
{
  if (m_timerIdForReconnect != 0)
  {
    killTimer(m_timerIdForReconnect);
    m_timerIdForReconnect = 0;
  }

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

  qDebug() << "Connected to server";

  Package* pack = new PackageForSignal(0, MessageType::MetaDataRequest, true);
  QByteArray message = m_protocol->encodeData(pack);

  m_socket->write(message);
}

void Client::startReconnectTimer()
{
  if (m_timerIdForSend != 0)
  {
    killTimer(m_timerIdForSend);
    m_timerIdForSend = 0;
  }

  qDebug() << "Disconnected from server";

  if (m_generator != nullptr)
  {
    delete m_generator;
    m_generator = nullptr;
  }

  if (m_timerIdForReconnect == 0)
  {
    m_timerIdForReconnect = startTimer(5000);
  }
}

void Client::readDataFromServer()
{
  QByteArray message = m_socket->readAll();

  m_protocol->m_buffer.append(message);

  Package* pack = m_protocol->decodeData();

  if (pack != nullptr)
  {
    switch (pack->m_type)
    {
    case MessageType::MetaDataResponse:
    {
      PackageForDataToGenerate* packageFromServer =
          dynamic_cast<PackageForDataToGenerate*>(pack);

      m_countOfBytes = packageFromServer->m_bytes;
      m_generator = SineGenerator::makeGenerator(packageFromServer->m_valueType);

      if (m_timerIdForSend == 0)
      {
        m_timerIdForSend = startTimer(2500);
      }
    }
    break;
    case MessageType::SinConfirmation:
    {
      if (m_timerIdForSend == 0)
      {
        m_timerIdForSend = startTimer(2500);
      }
    }
    break;
    default:
      qDebug() << "Unable to process this type of message:" << pack->m_type;
    }
  }
  if (pack != nullptr)
  {
    delete pack;
    pack = nullptr;
  }
}

void Client::closeSocket()
{
  qDebug() << "Unknown error";
  m_socket->close();
}

void Client::sendPackageToServer()
{
  if (m_timerIdForSend != 0)
  {
    killTimer(m_timerIdForSend);
    m_timerIdForSend = 0;
  }
  QByteArray data = m_generator->generateSineForType(m_countOfBytes);

  Package* pack = new PackageForGeneratedData(1, MessageType::SinAnswer, data);
  QByteArray message = m_protocol->encodeData(pack);

  qDebug() << this << "send: " << data;

  m_socket->write(message);

  if (!m_socket->waitForReadyRead(10000))
  {
    m_socket->close();
    if (m_timerIdForReconnect == 0)
    {
      m_timerIdForReconnect = startTimer(5000);
    }
  }
}

void Client::connectToServer()
{
  if ((m_socket->state() != QAbstractSocket::ConnectedState) &&
      (m_socket->state() != QAbstractSocket::ConnectingState))
  {
    qDebug() << "Connecting...";
    m_socket->connectToHost(QHostAddress(m_address), m_port, QTcpSocket::ReadWrite);
  }
}

void Client::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == m_timerIdForReconnect)
  {
    connectToServer();
  }
  else if (event->timerId() == m_timerIdForSend)
  {
    sendPackageToServer();
  }
  else
  {
    QObject::timerEvent(event);
  }
}
