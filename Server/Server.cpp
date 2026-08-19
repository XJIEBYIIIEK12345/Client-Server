#include "Server.h"
#include "Package.h"
#include "PackageForDataToGenerate.h"
#include "PackageForGeneratedData.h"
#include "PackageForSignal.h"
#include "PackageParser.h"
#include <QDataStream>
#include <QRandomGenerator64>
#include <netinet/in.h>
#include <netinet/tcp.h>

ClientData::ClientData()
  : m_id(0)
{}

ClientData::ClientData(quint32 _id)
  : m_id(_id)
{}

ClientData::~ClientData()
{
  if (m_parser != nullptr)
  {
    delete m_parser;
    m_parser = nullptr;
  }
}

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
  QTcpSocket* socket = new QTcpSocket(this);

  if (socket->setSocketDescriptor(socketDescriptor))
  {
    connect(socket, &QTcpSocket::readyRead, this, &Server::parseMessageFromClient);
    connect(socket, &QTcpSocket::disconnected, this, &Server::cleanClientData);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(socket, &QTcpSocket::errorOccurred, this, &Server::closeClientSocket);
    connect(socket, &QTcpSocket::stateChanged, this, &Server::logClientState);
    m_connectedClients.insert(socket, ClientData(m_connectedClients.size() + 1));
  }
  else
  {
    qDebug() << "System error:" << socket->errorString();
    delete socket;
    return;
  }

  int keepcnt = 3;
  int keepidle = 10;
  int keepintvl = 10;

  socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

  setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
  setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
  setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));

  m_connectedClients[socket].m_clientProtocol =
      IProtocol::makeProtocol(m_serverProtocol);

  qDebug() << "Client" << m_connectedClients[socket].m_id << " was connected";
}

void Server::parseMessageFromClient()
{
  QTcpSocket* dataSender = qobject_cast<QTcpSocket*>(sender());
  if (!dataSender)
    return;

  m_connectedClients[dataSender].m_clientProtocol->m_buffer.append(
      dataSender->readAll());

  Package* packFromClient =
      m_connectedClients[dataSender].m_clientProtocol->decodeData();

  if (packFromClient != nullptr)
  {
    switch (packFromClient->m_type)
    {
    case MessageType::MetaDataRequest:
    {
      QRandomGenerator* randomGenerator = QRandomGenerator::global();

      PackageParserType packageParserType = PackageParserType(
          randomGenerator->bounded(0, int(PackageParserType::Count) - 1));

      QString valueType = packageParserTypeName(packageParserType);
      m_connectedClients[dataSender].m_parser =
          PackageParser::makeParser(packageParserType);

      quint32 bytes = randomGenerator->bounded(0, 100000);

      Package* pack = new PackageForDataToGenerate(
          packFromClient->m_id + 1, MessageType::MetaDataResponse, valueType, bytes);
      QByteArray messageFromServer =
          m_connectedClients[dataSender].m_clientProtocol->encodeData(pack);

      dataSender->write(messageFromServer);
    }
    break;
    case MessageType::SinAnswer:
    {
      Package* pack = new PackageForSignal(packFromClient->m_id + 1,
                                           MessageType::SinConfirmation, true);
      QByteArray messageFromServer =
          m_connectedClients[dataSender].m_clientProtocol->encodeData(pack);

      dataSender->write(messageFromServer);

      PackageForGeneratedData* dataPackFromClient =
          dynamic_cast<PackageForGeneratedData*>(packFromClient);

      m_connectedClients[dataSender].m_parser->parseAndPrintPackage(
          dataPackFromClient->m_data, m_connectedClients[dataSender].m_id);

      if (dataPackFromClient != nullptr)
      {
        dataPackFromClient = nullptr;
      }
    }
    break;
    default:
      qDebug() << "Unable to process this type of message:"
               << packFromClient->m_type;
    }
  }
  if (packFromClient != nullptr)
  {
    delete packFromClient;
    packFromClient = nullptr;
  }
}

void Server::cleanClientData()
{
  QTcpSocket* disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
  if (!disconnectedSocket)
    return;

  m_connectedClients.remove(disconnectedSocket);
}

void Server::closeClientSocket()
{
  QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
  if (!socket)
    return;

  qDebug() << "Unknown error";
  socket->close();
}

void Server::logClientState()
{
  QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
  if (!socket)
    return;

  switch (socket->state())
  {
  case QAbstractSocket::HostLookupState:
    qDebug() << "Client" << m_connectedClients[socket].m_id
             << "is looking for host..." << "\n";
    break;
  case QAbstractSocket::ConnectingState:
    qDebug() << "Client" << m_connectedClients[socket].m_id
             << "is connecting to server..." << "\n";
    break;
  case QAbstractSocket::ConnectedState:
    qDebug() << "Client" << m_connectedClients[socket].m_id << "is connected"
             << "\n";
    break;
  case QAbstractSocket::ClosingState:
    qDebug() << "Client" << m_connectedClients[socket].m_id << "is closing..."
             << "\n";
    break;
  case QAbstractSocket::UnconnectedState:
    qDebug() << "Client" << m_connectedClients[socket].m_id << "is unconnected"
             << "\n";
    break;
  default:
    break;
  }
}
